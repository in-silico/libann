
#ifndef HOMOGRAPHY_H
#define HOMOGRAPHY_H

#include "img_lib.h"
#include "ransac.h"

using namespace std;
using namespace cv;

typedef Point2D<double> dpt;
typedef vector< dpt > vdpt;

class HomographyRansac : public Ransac {
    corresp orgPts;
    vdpt x, xp;
    Mat T, Tp;
    //model
    Mat H, bestH;

    HomographyRansac(int n, int m, int ndata, double rho) : Ransac(n,m,ndata,rho) {
    }

    void setCorresp(corresp myPairs) {
        dpt avgx(0,0), avgxp(0,0);
        int N = myPairs.size();
        for (int i=0; i<N; i++) {
            pair<ipt,ipt> curr = myPairs[i];
            orgPts.push_back(curr);
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
        s = 1/s; sp = 1/sp;
        for (int i=0; i<N; i++) {
            x[i] = x[i] * s;
            xp[i] = xp[i] * sp;
        }
        T = (Mat_<double>(3,3) << s,0,-s*avgx.x,0,s,-s*avgx.y,0,0,1);
        Tp = (Mat_<double>(3,3) << sp,0,-sp*avgxp.x,0,sp,-sp*avgxp.y,0,0,1);
    }

};

#endif
