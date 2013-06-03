
#ifndef IMG_LIB_H
#define IMG_LIB_H

#include <cv.h>
#include <highgui.h>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

//definitions

template<class T>
struct Point2D {
    T x,y;

    Point2D(T xx, T yy) {x=xx; y=yy;}
    ~Point2D() {}
    Point2D<T> operator+(Point2D<T> p) { return Point2D<T>(x+p.x, y+p.y); }
    Point2D<T> operator-(Point2D<T> p) { return Point2D<T>(x-p.x, y-p.y); }
    T dot(Point2D<T> p) { return x*p.x + y*p.y; }
    T norm() { return sqrt( dot(*this) ); }
    T dist(Point2D<T> p) { return (p - *this).norm(); }
};

typedef Point2D<int> ipt; //integer coordinate point 2D
typedef vector< Point2D<int> > vipt; //vector of ipt
typedef vector< pair<ipt,ipt> > corresp; //vector of pairs of ipt

template<class T>
class Image {
    bool isMax(int r, int c, int w);
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
    void nonMaxSupr(int wsize, T thresh, vipt &ans);
    Image& operator=(const Image &other);
//    void operator=(Image &other);
    void normalize(float maxVal=255);
    void transpose();
};

struct CorrespParams {
    int harrisW, nmsW, scoreW;
    double harrisK, nmsThresh;
    double matchThresh;
    char scoreMethod;

    CorrespParams() { 
        harrisW=5; nmsThresh=0.01; nmsW=7; nmsThresh=1e7; 
        matchThresh = 3; scoreMethod=0; scoreW=9;
    }
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
void harrisFilter(Image<T> &dest, Image<T> &org, int w = 5, double k=0.01);

template<class T>
double correspScore(Image<T> &img1, Image<T> &img2, ipt pt1, ipt pt2, CorrespParams &p);

template<class T>
void findCorrespondences(corresp &ans, Image<T> &img1, Image<T> &img2, CorrespParams &p);

template<class T>
void cart2pol(Image<T> &r, Image<T> &theta, Image<T> &x, Image<T> &y);

template<class T>
void img2cvimg(IplImage *ocv, Image<T> &img);

template<class T>
void cvimg2img(Image<T> &img, IplImage *ocv);

//Implementation

const double PI = acos(-1);

template<class T>
void gaussianFilter(Image<T> &dest, Image<T> &org, double sigma) {
    if (dest.rows != org.rows && dest.cols != org.cols) throw ImgError(10, "Output and input images must have the same dimensions on Gaussian Filter");
    int w = (int)round(3*sigma);
    if ( (w%2)==0 ) w++;
    int wh = w/2;
    double tss = 2*sigma*sigma;
    double coef = 1.0/(PI*tss);
    Image<T> tmp(w,w);
    double sum=0;
    for(int x=-wh; x<=wh; x++) for(int y=-wh;y<=wh;y++) {
        tmp(x+wh, y+wh) = coef*exp(-(x*x+y*y)/tss);
        sum += tmp(x+wh,y+wh);
    }
    for(int i=0; i<w; i++) for(int j=0; j<w; j++) tmp(i,j) = tmp(i,j) / sum;

    dest=org;
    dest.convolve(tmp);
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
void harrisFilter(Image<T> &dest, Image<T> &org, int w = 5, double k=0.01) {
    int n = org.rows; int m = org.cols;
    Image<T> dx(n,m);
    Image<T> dy(n,m);
    int wh = w/2;
    sobelFilter(dx,dy,org);    
    for(int row = 0; row < org.rows; ++row) {
        T* drow = dest.getRow(row);
        for(int col = 0; col < org.cols ; ++col) {
            double a=0, b=0, c=0;
            for(int i = -wh; i <= wh; ++i) {
                for(int j = -wh; j <= wh; ++j) {
                    double tx = dx.at(row+i, col+j), ty = dy.at(row+i,col+j);
                    a += tx*tx;
                    c += tx*ty;
                    b += ty*ty;
                }
            }
            double ab = a*b - c*c;
            double apb = a+b;
            drow[col] = (T)(ab - k*(apb*apb));
        }
    }  
}

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

template<class T>
double correspScore(Image<T> &img1, Image<T> &img2, ipt pt1, ipt pt2, CorrespParams &p) {
    int wh = p.scoreW;
    if (p.scoreMethod == 0) {
        //Square difference
        double tmp=0, dif;        
        for (int i=-wh; i<=wh; i++) {
            for (int j=-wh; j<=wh; j++) {
                T v1 = img1.at(pt1.x+i, pt1.y+j);
                T v2 = img2.at(pt2.x+i, pt2.y+j);
                dif = v1 - v2;
                tmp += (dif*dif);
            }
        }
        return -tmp;
    } else {
        //normalized cross-correlation
        return 0;
    }
}


template<class T>
void findCorrespondences(corresp &ans, Image<T> &img1, Image<T> &img2, CorrespParams &p) {
    Image<T> tmp1(img1.rows, img1.cols);
    Image<T> tmp2(img2.rows, img2.cols);

    harrisFilter(tmp1, img1, p.harrisW, p.harrisK);
    harrisFilter(tmp2, img2, p.harrisW, p.harrisK);

    vipt pt1, pt2;
    tmp1.nonMaxSupr(p.nmsW,p.nmsThresh,pt1);
    tmp2.nonMaxSupr(p.nmsW,p.nmsThresh,pt2);

    ans.clear();
    for (int i=0; i<pt1.size(); i++) {
        int best_ix=-1;
        double best_score=-1e100; //minus infinity
        for (int j=0; j<pt2.size(); j++) {
            double c_score = correspScore(img1, img2, pt1[i], pt2[j], p);
            if (c_score > best_score) {
                //this one is better
                best_ix = j;
                best_score = c_score;
            }
        }
        if (best_score > p.matchThresh) 
            ans.push_back( pair<ipt,ipt>(pt1[i],pt2[best_ix]) );
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
Image<T>& Image<T>::operator=(const Image &other){
    if (other.rows != rows || other.cols != cols)
        throw ImgError(11, "images of different dimensions on a copy operation.");
    rows = other.rows; cols = other.cols;
    memcpy(data, other.data, rows*cols*sizeof(T));
    return *this;
}

template<class T>
void Image<T>::transpose() {
    for(int i = 0; i< rows; i++){
        for(int j = i + 1; j< cols; j++){
            T tmp = this->at(i,j);
            this->at(i,j) = this->at(j,i);
            this->at(j,i) = tmp;
        }
    }
}

template<class T>
void Image<T>::normalize(float maxVal) {
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

template<class T>
bool Image<T>::isMax(int r, int c, int w) {
    int wm = w/2;
    for (int i=-wm; i<=wm; i++)
        for (int j=-wm; j<wm; j++) {
            if (at(r-i,c-j) > at(r,c)) return false;
        }
    return true;
}

template<class T>
void Image<T>::nonMaxSupr(int wsize, T thresh, vipt &ans) {
    ans.clear();
    for (int i=0; i<rows; i++) {
        T *r = getRow(i);
        for (int j=0; j<cols; j++) {
            if (r[j] > thresh && isMax(i,j,wsize)) {
                ans.push_back(ipt(i,j));
            }
        }
    }
}

#endif
