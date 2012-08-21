#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <map>
#include "matrix.h"

using namespace std;

#define rep(i,n) for(int i=0; i<(n); i++)
#define repf(i,a,b) for (int i=(a); i<=(b); i++)
#define repb(i,a,b) for(int i=(a); i>=(b); i--)
#define EPS 1E-8 

struct MatFileHead {
    int r,c;
    char flags;
};

void matCopy(Mat *ans, Mat *src) {
    ans->setSize(src->nrows(), src->ncols());
    rep(i,ans->nrows())
	rep(j,ans->ncols())
	    ans->get(i,j) = src->get(i,j);
}

void matCopyRow(Mat *ans, int rans, Mat *src, int rsrc) {
    if (ans->ncols() != src->ncols()) throw "Matrix width mismatch exception";
    int m = src->ncols();
    rep(i,m)
	ans->get(rans,i) = src->get(rsrc, i);
}

void Mat::save(const char *fname) {
    FILE *f = fopen(fname, "wb");
    MatFileHead h;
    h.r = r; h.c = c; h.flags = flags;
    fwrite(&h,sizeof(h),1,f);
    fwrite(data,sizeof(double),r*c,f);
    fclose(f);
}

void Mat::load(const char *fname) {
    FILE *f = fopen(fname, "rb");
    MatFileHead h;
    if (fread(&h,sizeof(h),1,f) != 1) throw "Wrong matrix format exception";
    setSize(h.r,h.c); flags = h.flags;
    if (fread(data,sizeof(double),r*c,f) != (unsigned int)(r*c)) 
	throw "Wrong matrix format exception";
    fclose(f);
}

void Mat::setSize(int w, int h) { 
    if (w*h > maxdata) throw "Insufficient space for this matrix exception";
    r=w; c=h; flags=0; 
}


void matMult(Mat *ans, Mat *a, Mat *b) {
    int nr = a->nrows(), nc = b->ncols();
    if (nr*nc > ans->maxdata) throw "Matrix multiplication overflow";
    if (a->ncols() != b->nrows()) throw "Wrong dimensions in matrix multiplication";
    Mat *tmp = new Mat(nr*nc);
    tmp->setSize(nr, nc);
    int nk = a->ncols();
    rep(i,nr) {
	rep(j,nc) {
	    double x = 0;
	    rep(k,nk) x += a->get(i,k)*b->get(k,j);
	    tmp->get(i,j) = x;
	}
    }
    matCopy(ans,tmp);
    delete tmp;
}

//Swaps rows r1 and r2 in place
void matSwapRows(Mat *ans, int r1, int r2) {
    rep(j,ans->ncols()) {
        double tmp = ans->get(r1,j);
        ans->get(r1,j) = ans->get(r2,j);
        ans->get(r2,j) = tmp;
    }
}

//Scales row r of matrix ans in place
void matScaleRow(Mat *ans, int r, double scale) {
    rep(j,ans->ncols()) ans->get(r,j) = ans->get(r,j)*scale;
}

//Operates ans in place as r1 = r1 + scale*r2
void matAddRows(Mat *ans, double scale, int r1, int r2) {
    rep(j,ans->ncols()) {
        ans->get(r1,j) += ans->get(r2,j)*scale;
    }
}

bool matInv(Mat *ans, Mat *x) {
    int n=x->nrows(), m=x->ncols();
    if (n != m) throw "Inverting non-square matrix exception";
    
    //set tmp matrix as x extended with identity
    Mat *tmp = new Mat(n*n*2);
    tmp->setSize(n,2*n);
    rep(i,n) rep(j,n) tmp->get(i,j) = x->get(i,j);
    rep(i,n) rep(j,n) tmp->get(i,j+n) = (i==j);

    rep(j,n) {
        repf(k,j+1,n-1) 
            if (fabs(tmp->get(k,j)) > fabs(tmp->get(j,j)))
                matSwapRows(tmp,j,k);
        if ( fabs(tmp->get(j,j)) < EPS ) {
            //Matrix without inverse
            delete tmp;
            return false;
        }

        //set thisone to one 
        matScaleRow(tmp, j, 1.0/tmp->get(j,j));

        //set the others to zero
        rep(k,n) {
            if (k!=j) matAddRows(tmp,-tmp->get(k,j),k,j);
        }
    }

    rep(i,n) rep(j,n) ans->get(i,j) = tmp->get(i,j+n);

    delete tmp;
    return true;
}

bool matPinv(Mat *ans, Mat *x) {
    Mat *tmp = new Mat(ans->maxdata);
    matCopy(tmp, x);
    x->transpose();
    
    matMult(tmp,x,tmp); //tmp = x'*x
    bool invertible = matInv(tmp,tmp); //tmp = inv(x'*x)
    matMult(tmp,tmp,x); //tmp = inv(x'*x)*x'
    
    x->transpose();
    matCopy(ans, tmp);
    delete tmp;
    return invertible;
}

void matSetZero(Mat *ans) {
    rep(i,ans->nrows()) 
	rep(j,ans->ncols())
	    ans->get(i,j)=0;
}

