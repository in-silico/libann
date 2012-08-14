
#include "kmeans.h"
#include <limits>
#define rep(i,n) for(int i=0; i<(n); i++)

const double inf = std::numeric_limits<double>::max(); 

using namespace LibAnn;

/* Space for private helper functions here */


/* Public functions here */

int kmeans(Mat *centers, const Mat *x, const Mat *initial_centroids, int maxIter) {
}

void kmeansInit(Mat *centroids, const Mat *x, int k) {
}

void normalize(Mat *dest, const Mat *org) {
}


/**
    Computes the N-dimentional "distance" 
*/
double distance(Mat *a, Mat *b,const int a_row,const int b_row){
    if(a->ncols()!=b->ncols())
        throw "The vectors have different dimensions";
    double acum=1;
    rep(j,a->ncols()){
        acum*= (a->get(a_row,j) - b->get(b_row,j) );
    }
    return acum;
}

void findClosestCentroids(int *idx, Mat *x, Mat *centroids) {
    int x_r=x->nrows() , cent_r= centroids->nrows(), row_min=0;
    double m,tmp;
    rep(i,x_r){
        m = inf;
        rep(j,cent_r){
            tmp = distance(x,centroids,i,j);
            if(tmp<m){
                m=tmp;
                row_min=j;    
            }
        }
        idx[i]=row_min;
    }
}

void computeCentroids(Mat *centroids, const Mat* x, int *idx, int k) {
}
