
#ifndef HOMOGRAPHY_H
#define HOMOGRAPHY_H

#include "img_lib.h"
#include "ransac.h"

#ifndef rep
#define rep(i,n) for(int i=0; i<(n); i++)
#endif

using namespace std;
using namespace cv;

typedef Point2D<double> dpt;
typedef vector< dpt > vdpt;

class HomographyRansac : public Ransac {
public:
    //corresp orgPts;
    vdpt x, xp;
    Mat T, Tp;
    //model
    Mat H, bestH;
    Mat vecH; //h as a single column vector

    HomographyRansac(int n, int m, int ndata, double rho) : Ransac(n,m,ndata,rho) {
        H.create(3,3,CV_64F); bestH.create(3,3,CV_64F);
        vecH.create(9,1,CV_64F);
    }

    void fillRows(double *r1, double *r2, int ix) {
        double xi[3]; xi[0]=x[ix].x; xi[1]=x[ix].y; xi[2]=1;
        double wip = 1, xip= xp[ix].x, yip=xp[ix].y;
        rep(j,3) r1[j]=0;
        rep(j,3) r1[3+j] = -wip*xi[j];
        rep(j,3) r1[6+j] = yip*xi[j];

        rep(j,3) r2[j] = wip*xi[j];
        rep(j,3) r2[3+j] = 0;
        rep(j,3) r2[6+j] = -xip*xi[j];
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
        double *ans = svd.vt.ptr<double>(8); //must be 9x9

        rep(i,9) vecH.at<double>(i,0) = ans[i];
        rep(i,3) {
            double *hrow = H.ptr<double>(i);
            rep(j,3) hrow[j] = ans[3*i + j];
        }
    }

    double compError(int ix) {
        //this returns the algebraic error
        Mat A(2,9,CV_64F);
        cmpA(A,&ix,1);
        Mat err = A*vecH;
        return err.dot(err);
    }

    void setCorresp(corresp &myPairs) {
        dpt avgx(0,0), avgxp(0,0);
        int N = myPairs.size();
        for (int i=0; i<N; i++) {
            pair<ipt,ipt> curr = myPairs[i];
            //orgPts.push_back(curr);
            dpt tmp1(curr.first.x,curr.first.y);
            dpt tmp2(curr.second.x,curr.second.y);
            avgx = avgx + tmp1;
            avgxp = avgxp + tmp2;
        }
        //normalization process
        avgx = avgx * ( ((double)1)/N );
        avgxp = avgxp * ( ((double)1)/N );
        double s=0,sp=0;
        for (int i=0; i<N; i++) {
            pair<ipt,ipt> curr = myPairs[i];
            dpt tmp1(curr.first.x,curr.first.y);
            dpt tmp2(curr.second.x,curr.second.y);

            x.push_back(tmp1 - avgx);
            xp.push_back(tmp2 - avgxp);
            s += x[i].norm(); sp += xp[i].norm();
        }
        double tmp = sqrt(2);
        s = tmp*N/s; sp = tmp*N/sp;
        for (int i=0; i<N; i++) {
            x[i] = x[i] * s;
            xp[i] = xp[i] * sp;
        }
        T = (Mat_<double>(3,3) << s,0,-s*avgx.x,0,s,-s*avgx.y,0,0,1);
        Tp = (Mat_<double>(3,3) << sp,0,-sp*avgxp.x,0,sp,-sp*avgxp.y,0,0,1);
    }

    void saveCurrentModel() {
        //save the current model in unnormalized form
        bestH = Tp.inv()*H*T;
    }

    void useSavedModel() {
        bestH.copyTo(H);
    }

};

#undef rep
#endif
