
#ifndef IMG_LIB_H
#define IMG_LIB_H

#include <cv.h>
#include <highgui.h>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

struct ImgError {
    int code;
    string message;

    ImgError(int a, string b): code(a), message(b) {}
};


template<class T>
class Image {
    bool isMax(int r, int c, int w);
public:   
    T *data;
    int rows,cols;

    Image(int _rows, int _cols): rows(_rows), cols(_cols){
        data = new T[rows*cols];
    }
    
    ~Image(){
        delete [] data;
    }

    T &at(int i, int j){
        if (i<0) i = rows + i ;
        if (j<0) j = cols + j;
        if (i>=rows) i= i%rows;
        if (j>=cols) j= j%cols;
        return data[i*cols + j];
    }

    T &operator()(int i, int j){
        if (i<0 || j<0 || i>=rows || j>=cols) throw ImgError(1,"Out of bound index exception");
        return data[i*cols + j];
    }
    
    T *getRow(int r){
        return &data[r*cols];
    }
    
    /**
    * No usar !!!
    **/
    vector<T> *getCol(int c){
        vector<T> col(rows);
        for(int i = 0; i < rows; ++i)
            col[i] = data[i*cols + c];
            
        return col;
    }
    
    void convolve(Image &kernel){
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

    void normalize(float maxVal=255){
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
    void transpose(){
        for(int i = 0; i< rows; i++){
            for(int j = i + 1; j< cols; j++){
                T tmp = this->at(i,j);
                this->at(i,j) = this->at(j,i);
                this->at(j,i) = tmp;
            }
        }
        int t = rows;
        rows =  cols;
        cols = t;
    }
};

template<class T>
void cart2pol(Image<T> &r, Image<T> &theta, Image<T> &x, Image<T> &y) {
    if (!(r.rows == theta.rows && theta.rows == x.rows && x.rows == y.rows))
        throw new ImgError(12, "the images dont have the same dimensions on cart2pol");
    int rows = r.rows;
    int cols = r.cols;
    for(int i = 0; i < rows; i++){
        T* rx = x.getRow(i);
        T* ry = y.getRow(i);
        T* rr = r.getRow(i);
        T* rt = theta.getRow(i);
        for(int j = 0; j < cols; j++){
            rr[j] = sqrt( (rx[j] * rx[j]) + (ry[j] * ry[j]));
            rt[j] = atan2(ry[j], rx[j]);
        }
    }
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

#endif
