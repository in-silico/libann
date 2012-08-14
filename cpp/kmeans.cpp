#include<cmath>
#include "kmeans.h"

using namespace LibAnn;

/* Space for private helper functions here */


/* Public functions here */

int kmeans(Mat *centers, const Mat *x, const Mat *initial_centroids, int maxIter) {
}

void kmeansInit(Mat *centroids, const Mat *x, int k) {
}

void std(Mat *stdev, Mat *mean, Mat *x){
    double tmp;
    //mean
    for(int j=0; j < x->ncols() ; j++){
        tmp=0;
        for(int i=0; i < x->nrows() ; i++){
            tmp += x->get(i,j);
        }
        mean->get(0,j)= tmp/(x->nrows());
    }
    //standar deviation
    for(int j=0; j < x->ncols() ; j++){
        tmp=0;
        for(int i=0; i < x->nrows() ; i++){
            tmp += pow(x->get(i,j),2);
        }
        stdev->get(0,j)= (tmp/x->nrows()) - pow(mean->get(0,j),2);
    }

}
void normalize(Mat *dest, const Mat *org) {
}

void findClosestCentroids(int *idx, const Mat *x, const Mat *centroids) {
}

void computeCentroids(Mat *centroids, const Mat* x, int *idx, int k) {
}
