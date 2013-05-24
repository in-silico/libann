
#ifndef IMG_LIB_H
#define IMG_LIB_H

#include <cv.h>
#include <highgui.h>
#include <string>

using namespace std;

//definitions

template<class T>
class Image {
    T *data;
    int rows,cols;
    
    T &at(int i, int j);
public:
    Image(int row, int cols);
    ~Image();
    T &operator()(int i, int j);
    T *getRow(int r);
    void convolve(Image &kernel);
    void operator=(Image &other);
    void normalize(int minVal=0, int maxVal=255);
};

struct ImgError {
    int code;
    string message;

    ImgError(int code, const char *error) {
        this->code = code; message = string(error);
    }
};

template<class T>
void gaussianFilter(Image<T> *dest, Image<T> *org, double sigma);

template<class T>
void sobelFilter(Image<T> *dx, Image<T> *dy, Image<T> *org);

template<class T>
void harrisFilter(Image<T> *dest, Image<T> *org, int w, double k=0.01);

template<class T>
void cart2pol(Image<T> *r, Image<T> *theta, Image<T> *x, Image<T> *y);

template<class T>
void img2mat(IplImage *ocv, Image<T> *img);

template<class T>
void mat2img(Image<T> *img, IplImage *ocv);

//Implementation

template<class T>
void gaussianFilter(Image<T> *dest, Image<T> *org, double sigma) {

}

template<class T>
void sobelFilter(Image<T> *dx, Image<T> *dy, Image<T> *org) {

}

template<class T>
void harrisFilter(Image<T> *dest, Image<T> *org, int w, double k=0.01) {

}

template<class T>
void cart2pol(Image<T> *r, Image<T> *theta, Image<T> *x, Image<T> *y) {

}

template<class T>
void img2mat(IplImage *ocv, Image<T> *img) {

}

template<class T>
void mat2img(Image<T> *img, IplImage *ocv) {

}


template<class T>
T& Image<T>::at(int i, int j) {
    if (i<0) i=0;
    if (j<0) j=0;
    if (i>=rows) i=rows-1;
    if (j>=cols) j=cols-1;
    return data[i*rows + j];
}

template<class T>
T& Image<T>::operator()(int i, int j) {
    if (i<0 || j<0 || i>=rows || j>=cols) throw ImgError(1,"Out of bound index exception");
    return data[i*rows + j];
}

template<class T>
Image<T>::Image(int row, int cols) {
    this->row = row; this->cols = cols;
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
void Image<T>::operator=(Image &other) {
    rows = other.rows; cols = other.cols;
    memcpy(data, other.data, rows*cols*sizeof(T));
}

#endif
