
#include <cv.h>
#include <highgui.h>
#include <cmath>

#define DEBUG false
#define rep(i,n) for(int i=0; i<(n); i++)
#define repf(i,a,b) for (int i=(a); i<=(b); i++)
#define repb(i,a,b) for(int i=(a); i>=(b); i--)
#define PI (acos(-1))

typedef float pix;

const double __sobel[3][3] = {{1,0,-1},{2,0,-2},{1,0,-1}};

void genGaussianKernel(CvMat *dest, int w, double sigma) {
    int wh = w/2;
    double tss = 2*sigma*sigma;
    double coef = 1.0/(PI*tss);
    double tmp[w][w];
    double sum=0;
    repf(x,-wh,wh) repf(y,-wh,wh) {
        tmp[x+wh][y+wh] = coef*exp(-(x*x+y*y)/tss);
        sum += tmp[x+wh][y+wh];
    }
    rep(i,w) rep(j,w) cvSetReal2D(dest,i,j,tmp[i][j]/sum);
}

double at(int x, int y, CvMat *org) {
    if (x<0) x=0;
    if (y<0) y=0;
    if (x>=org->width) x = org->width-1;
    if (y>=org->height) y = org->height-1;
    pix *r = (pix*)(org->data.ptr + y*org->step);
    return r[x];
}

void cart2pol(CvMat *r, CvMat *theta, CvMat *x, CvMat *y) {
    int n = r->height, m = r->width; //must be equal for all received matrices
    rep(i,n) {
        pix *dx = (pix*)(x->data.ptr + i*x->step);
        pix *dy = (pix*)(y->data.ptr + i*y->step);
        pix *dr = (pix*)(r->data.ptr + i*r->step);
        pix *dt = (pix*)(theta->data.ptr + i*theta->step);
        rep(j,m) {
            dr[j] = sqrt(dx[j]*dx[j] + dy[j]*dy[j]);
            dt[j] = atan2(dy[j],dx[j]);
        }
    }
}

void convolve(CvMat *dest, CvMat *org, CvMat *filter) {
    int n = dest->height, m = dest->width;
    int wn = filter->height, wm = filter->width;
    if (n != org->height || m != org->width) throw "Org and dest must have the same dimensions";
    rep(i,n) {
        pix *r = (pix*)(dest->data.ptr + i*dest->step);
        rep(j,m) {
            double ans = 0;
            rep(wi,wn) rep(wj,wm) {
                ans += at(wj,wi,filter)*at(j-(wm/2)+wj,i-(wn/2)+wi,org);
            }
            r[j] = (pix)ans;
        }
    }
}

void sobelFilter(CvMat *dx, CvMat *dy, CvMat *org) {
    CvMat *f[2];
    rep(i,2) f[i] = cvCreateMat(3,3,CV_32F);
    rep(x,3) rep(y,3) {
        cvSetReal2D(f[0],x,y,__sobel[x][y]);
        cvSetReal2D(f[1],x,y,__sobel[y][x]);
    }
    convolve(dx,org,f[0]);
    convolve(dy,org,f[1]);
    rep(i,2) cvReleaseMat( &f[i] );
}

