
#include "ransac.h"
#include <cstdio>
#include <iostream>
#include <cv.hpp>
#include <cxcore.hpp>

#define rep(i,n) for(int i=0; i<(n); i++)
#define repf(i,a,b) for (int i=(a); i<=(b); i++)
#define repb(i,a,b) for(int i=(a); i>=(b); i--)

using namespace cv;
using namespace std;

void pmat(Mat x) {
    rep(i,x.rows) {
        rep(j,x.cols) {
            cout << x.at<double>(i,j) << " ";
        }
        cout << endl;
    }
}

//solve the system Aw = b with least squares
class RansacLineReg : public Ransac {
public:
    int dims;
    Mat w;
    Mat wbest;
    Mat X; //matrix where A is taken
    Mat b;

    RansacLineReg(int n, int m, int ndata, double rho) : Ransac(n,m,ndata,rho) {
    }

    void computeModel(int *indexes, int nindexes) {
        Mat A(nindexes,dims,CV_64F);
        Mat my_b(nindexes,1,CV_64F);
        rep(i,nindexes) {
            Mat Ai = A.row(i);
            X.row(indexes[i]).copyTo(Ai);
            my_b.at<double>(i,0) = b.at<double>(indexes[i],0);
        }
        Mat Ap(dims,nindexes,CV_64F); Mat tmp(dims,dims,CV_64F);
        invert(A.t()*A,tmp);
        Ap = tmp*A.t();
        w = Ap*my_b;
        //pmat(w);
    }

    double compError(int ix) {
        Mat d = X.row(ix)*w - b.row(ix);
        double err = d.at<double>(0,0);
        return sqrt(err*err);
    }

    void saveCurrentModel() {
        wbest = w.clone();
    }

    void useSavedModel() {
        w = wbest;
    }
};

void pdvec(double *vec, int n) {
    rep(i,n) printf("%.5lf ",vec[i]);
    printf("\n");
}

//int tmp[7] = {3,1,6,0,2,4,5};

void test1() {
    freopen("Quizz09data.csv","r",stdin);
    int n; 
    cin >> n;
    double y0[n], y[n], x[n];
    rep(i,n) cin >> x[i];
    rep(i,n) cin >> y[i];
    rep(i,n) cin >> y0[i];
    
    Mat X(n,3,CV_64F); Mat b(n,1,CV_64F);
    rep(i,n) {
        X.at<double>(i,0) = 1;
        X.at<double>(i,1) = sin(x[i]/10.0);
        X.at<double>(i,2) = sin(x[i]/20.0);
        b.at<double>(i,0) = y0[i];
    }
    RansacLineReg r(15,40,n,0.25);
    r.X = X; r.b = b; r.dims=3;
    r.ransac();
    printf("W vector:\n");
    pmat(r.wbest);
    int *ix = new int[n];
    int m = r.supportSize(ix);
    int tmp0=0, tmp1=0;
    double inlx[m],inly[m],outlx[n-m],outly[n-m];
    rep(i,n) {
        if (ix[tmp0]==i) {
            inlx[tmp0] = x[i]; inly[tmp0] = y0[i];
            tmp0++;
        } else {
            outlx[tmp1] = x[i]; outly[tmp1] = y0[i];
            tmp1++;
        }
    }

    printf("Inliers X:\n");
    pdvec(inlx,tmp0); pdvec(inly,tmp0);
    printf("Outliers X:\n");
    pdvec(outlx,tmp1); pdvec(outly,tmp1);
    delete [] ix;
}

int main() {
    test1();
}
