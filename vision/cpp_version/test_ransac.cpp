
#include "ransac.h"
#include <cstdio>
#include <cv.h>

#define rep(i,n) for(int i=0; i<(n); i++)
#define repf(i,a,b) for (int i=(a); i<=(b); i++)
#define repb(i,a,b) for(int i=(a); i>=(b); i--)

using namespace cv;

//solve the system Aw = b with least squares
class RansacLineReg : Ransac {
public:
    int dims;
    double *w;
    double *wbest;
    CvMat *X; //matrix where A is taken
    double *b;

    void computeModel(int *indexes) {
        Mat A(n,dims,CV_64F);
        rep(i,n) {
            double *r1 = (double*)(X->data.ptr + (indexes[i])*X->step);
            double *r2 = (double*)(A->data.ptr + i*A->step);
            rep(j,dims) r2[j] = r1[j];
        }

        
    }
};

int main() {
    Mat A(10,10,CV_64F);
    printf("Hello world\n");
}
