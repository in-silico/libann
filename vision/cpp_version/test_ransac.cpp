
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
class RansacLineReg : Ransac {
public:
    int dims;
    Mat w;
    Mat wbest;
    Mat X; //matrix where A is taken
    Mat b;

    RansacLineReg(int n, int m, int ndata, double rho) : Ransac(n,m,ndata,rho) {
    }

    void computeModel(int *indexes) {
        Mat A(n,dims,CV_64F);
        Mat my_b(n,1,CV_64F);
        rep(i,n) {
            Mat Ai = A.row(i);
            X.row(indexes[i]).copyTo(Ai);
            my_b.at<double>(i,0) = b.at<double>(indexes[i],0);
        }
        Mat Ap(dims,n,CV_64F); Mat tmp(dims,dims,CV_64F);
        invert(A.t()*A,tmp);
        Ap = tmp*A.t();
        w = Ap*my_b;
        //pmat(w);
    }

    double compError(int ix) {
        Mat y = X.row(ix)*w;
        return y.at<double>(0,0);
    }

    void saveCurrentModel() {
        wbest = w.clone();
    }
};

//int tmp[7] = {3,1,6,0,2,4,5};

void test1() {
    freopen("Quizz09data.csv","r",stdin);
    int n; 
    cin >> n;
    double y0[n], y[n], x[n];
    rep(i,n) cin >> x[i];
    rep(i,n) cin >> y[i];
    rep(i,n) cin >> y0[i];
    
    Mat X(n,3,CV_64F); Mat b(n,1,CV64_F);
    rep(i,n) {
        X.at<double>(i,0) = 1;
        X.at<double>(i,1) = sin(x[i]/10.0);
        X.at<double>(i,2) = sin(x[i]/20.0);
        b.at<double>(i,0) = y0[i];
    }
    RansacLineReg r(10,30,n,0.3);
    r.X = X; r.b = b; r.dims=3;
    r.ransac();
    pmat(r.wbest);
}

int main() {
    int n,m;
    cin >> n >> m;
    Mat X(n,m,CV_64F); Mat b(n,1,CV_64F);
    rep(i,n) {
        rep(j,m) cin >> X.at<double>(i,j);
        cin >> b.at<double>(i,0);
    }
    RansacLineReg r(7,n-1,n,0.1);
    r.X = X; r.b = b; r.dims=m;
    //r.computeModel(tmp);
    r.ransac();
}
