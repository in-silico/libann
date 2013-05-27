
#ifndef IMG_LIB_H
#define IMG_LIB_H

#include <cv.h>
#include <highgui.h>
#include <string>
#include <algorithm>
#include <cstdio>

using namespace std;

//definitions

template<class T>
class Image {
public:   
    T &at(int i, int j);
    T *data;
    int rows,cols;

    Image(int row, int cols);
    ~Image();
    T &operator()(int i, int j);
    T *getRow(int r);
    T *getCol(int c);
    void convolve(Image &kernel);
    void operator=(Image &other);
    void normalize(int maxVal=255);
};

struct ImgError {
    int code;
    string message;

    ImgError(int code, const char *error) {
        this->code = code; message = string(error);
    }
};

template<class T>
void gaussianFilter(Image<T> &dest, Image<T> &org, double sigma);

template<class T>
void sobelFilter(Image<T> &dx, Image<T> &dy, Image<T> &org);

template<class T>
void harrisFilter(Image<T> &dest, Image<T> &org, int w, double k=0.01);

template<class T>
void cart2pol(Image<T> &r, Image<T> &theta, Image<T> &x, Image<T> &y);

template<class T>
void img2cvimg(IplImage *ocv, Image<T> &img);

template<class T>
void cvimg2img(Image<T> &img, IplImage *ocv);

//Implementation

template<class T>
void gaussianFilter(Image<T> &dest, Image<T> &org, double sigma) {

}

int gaussian[3] = {1 , 2, 1};
int diff[3] = {1, 0, -1};

template<class T>
void sobelFilter(Image<T> &dx, Image<T> &dy, Image<T> &org) {
    int tamgaussian = sizeof(gaussian) / sizeof(int);
    int tamdiff = sizeof(diff) / sizeof(int);
    Image<T> gaussian_kernel_x(1, tamgaussian);
    Image<T> kernel_dx(tamdiff, 1);
    Image<T> gaussian_kernel_y(tamgaussian, 1);
    Image<T> kernel_dy(1, tamdiff);
    for(int i = 0; i < tamgaussian; i++){
        gaussian_kernel_x(0, i) = gaussian[i];
        gaussian_kernel_y(i, 0) = gaussian[i];
    }
    for(int i = 0; i < tamdiff; i++){
        kernel_dx(i, 0) = diff[i];
        kernel_dy(0, i) = diff[i];
    }
    dx = org;
    dx.convolve(gaussian_kernel_x);
    dx.convolve(kernel_dx);
    dy = org;
    dy.convolve(gaussian_kernel_y);
    dy.convolve(kernel_dy);
}

template<class T>
void harrisFilter(Image<T> &dest, Image<T> &org, int w, double k=0.01) {

}

template<class T>
void cart2pol(Image<T> &r, Image<T> &theta, Image<T> &x, Image<T> &y) {

}

template<class T>
void cvimg2img(Image<T> &img, IplImage *ocv) {
    int n = ocv->height, m = ocv->width, nch = ocv->nChannels;
    int ch=0;
    for(int i=0; i<n; i++) {
        uchar *orow = (uchar*)(ocv->imageData + i*ocv->widthStep);
        T *drow = img.getRow(i);
        for (int j=0; j<m; j++) {
            drow[j] = orow[j*nch + ch];
        }
    }
}

template<class T>
void img2cvimg(IplImage *ocv, Image<T> &img) {
    int n = ocv->height, m = ocv->width, nch = ocv->nChannels;
    int ch=0;
    for(int i=0; i<n; i++) {
        uchar *drow = (uchar*)(ocv->imageData + i*ocv->widthStep);
        T *orow = img.getRow(i);
        for(int j=0; j<m; j++) {
            T x = orow[j];
            if (x<0) x=0; if(x>255) x=255;
            drow[j*nch + ch] = x;
        }
    }
}


template<class T>
T& Image<T>::at(int i, int j) {
    if (i<0) i=0;
    if (j<0) j=0;
    if (i>=rows) i=rows-1;
    if (j>=cols) j=cols-1;
    return data[i*cols + j];
}

template<class T>
T& Image<T>::operator()(int i, int j) {
    if (i<0 || j<0 || i>=rows || j>=cols) throw ImgError(1,"Out of bound index exception");
    return data[i*cols + j];
}

template<class T>
Image<T>::Image(int row, int cols) {
    this->rows = row; this->cols = cols;
    data = new T[rows*cols];
}

template<class T>
Image<T>::~Image() {
    delete [] data;
}

template<class T>
T* Image<T>::getRow(int r) {
    return &data[r*cols];
}

template<class T>
T* Image<T>::getCol(int c){
    T * col =  new T[rows];
    for(int i = 0; i < rows; ++i)
        col[i] = data[i*cols + c];
        
    return col;
}

template<class T>
void Image<T>::operator=(Image &other) {
    if (other.rows != rows || other.cols != cols)
        throw ImgError(11, "images of different dimensions on a copy operation.");
    rows = other.rows; cols = other.cols;
    memcpy(data, other.data, rows*cols*sizeof(T));
}

template<class T>
void Image<T>::normalize(int maxVal) {
    double maxv=-1e100, minv=1e100;
    for(int i=0; i<rows; i++) {
        T *orow = this->getRow(i);
        for(int j=0; j<cols; j++) {
            maxv = max((double)orow[j],maxv);
            minv = min((double)orow[j],minv);
        }
    }
    double scale = maxVal/(maxv-minv);
    for(int i=0; i<rows; i++) {
        T *drow = this->getRow(i);
        for(int j=0; j<cols; j++) {
            drow[j] = (T)( (drow[j]-minv)*scale );
        }
    }
}

template<class T>
void Image<T>::convolve(Image &kernel) {
    int wn = kernel.rows, wm = kernel.cols;
    Image tmp(rows, cols);
    for(int i=0; i<rows; i++) {
        T *r = tmp.getRow(i);
        for(int j=0; j<cols; j++) {
            T ans = 0;
            for(int wi=0; wi<wn; wi++) {
                for (int wj=0; wj<wm; wj++) {
                    ans += kernel.at(wi,wj)*at(i-(wn/2)+wi,j-(wm/2)+wj);
                }
            }
            r[j] = ans;
        }
    }
    T* tmp2 = data;
    data = tmp.data;
    tmp.data = tmp2;
}

#endif
