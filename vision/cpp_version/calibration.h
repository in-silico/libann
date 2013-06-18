
#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "img_lib.h"
#include "ransac.h"

#ifndef rep
#define rep(i,n) for(int i=0; i<(n); i++)
#endif

using namespace std;
using namespace cv;

class TsaiCalibRansac : public Ransac {
public:
    //corresp orgPts;
    Mat X3d, X2d; //matrices with the correspondences of 3D-2D pts in cartesian coordinates
    Mat T, U; //normalization matrices of 2D and 3D respectively

    //model
    Mat P, bestP;
    Mat vecP; //h as a single column vector

    TsaiCalibRansac(int n, int m, int ndata, double rho) : Ransac(n,m,ndata,rho) {
        P.create(3,4,CV_64F); bestP.create(3,4,CV_64F);
        vecP.create(12,1,CV_64F);
    }

    void fillRows(double *r1, double *r2, int ix) {
        double *x3c = X3d.ptr<double>(ix);
        double *x2c = X2d.ptr<double>(ix);
        double x3h[4];
        rep(i,3) x3h[i]=x3c[i];
        x3h[3]=1;
        double wip = 1, xip=x2c[0], yip=x2c[1];
        rep(j,4) r1[j]=0;
        rep(j,4) r1[3+j] = -wip*x3h[j];
        rep(j,4) r1[6+j] = yip*x3h[j];

        rep(j,4) r2[j] = wip*x3h[j];
        rep(j,4) r2[3+j] = 0;
        rep(j,4) r2[6+j] = -xip*x3h[j];
    }

    void cmpA(Mat &A,int *indexes, int nindexes) {
        for (int i=0; i<nindexes; i++) {
            double *r1 = A.ptr<double>(2*i);
            double *r2 = A.ptr<double>(2*i + 1);
            fillRows(r1,r2,indexes[i]);
        }
    }

    void computeModel(int *indexes, int nindexes) {
        Mat A(2*nindexes,9,CV_64F);
        cmpA(A, indexes, nindexes);
        //do svd and take last column of V, that should be the last row of V^T
        SVD svd(A);
        double *ans = svd.vt.ptr<double>(svd.vt.rows - 1); 

        rep(i,12) vecP.at<double>(i,0) = ans[i];
        rep(i,3) {
            double *hrow = P.ptr<double>(i);
            rep(j,4) hrow[j] = ans[4*i + j];
        }
    }

    double compError(int ix) {
        //this returns the algebraic error
        Mat A(2,12,CV_64F);
        cmpA(A,&ix,1);
        Mat err = A*vecP;
        return err.dot(err);
    }

    void setCorresp(Mat x3d, Mat x2d) {
        double mu3[3],mu2[2];
        rep(i,3) mu3[i] = mean(x3d.col(i))[0];
        rep(i,2) mu2[i] = mean(x2d.col(i))[0];
        int N = x3d.rows;
        if (N != x2d.rows) throw ImgError(15,"The number of rows doesn't match in a correspondence");
        X3d.create(N,3,CV_64F);
        X2d.create(N,2,CV_64F);
        double s2=0,s3=0;
        rep(i,3) X3d.col(i) = x3d.col(i)-mu3[i];
        rep(i,2) X2d.col(i) = x2d.col(i)-mu2[i];
        rep(i,N) {
            s2 += norm(X2d.row(i));
            s3 += norm(X3d.row(i));
        }
        double tmp2 = sqrt(2), tmp3 = sqrt(3);
        s2 = tmp2*N/s2; s3 = tmp3*N/s3;
        for (int i=0; i<N; i++) {
            X3d.row(i) = X3d.row(i) * s3;
            X2d.row(i) = X2d.row(i) * s2;
        }
        T = (Mat_<double>(3,3) << s2,0,-s2*mu2[0],0,s2,-s2*mu2[1],0,0,1);
        U = (Mat_<double>(4,4) << s3,0,0,-s3*mu3[0], 0,s3,0,-s3*mu3[1], 0,0,s3,-s3*mu3[2], 0,0,0,1);
    }

    void saveCurrentModel() {
        //save the current model in unnormalized form
        bestP = T.inv()*P*U;
    }

    void useSavedModel() {
        bestP.copyTo(P);
    }

};


#undef rep
#endif
