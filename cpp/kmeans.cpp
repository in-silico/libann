
#include "kmeans.h"
#include <cstring>

//using namespace LibAnn;

/* Space for private helper functions here */


/* Public functions here */
namespace LibAnn {
    int kmeans(Mat *centers, const Mat *x, const Mat *initial_centroids, int maxIter) {
    }

    void kmeansInit(Mat *centroids, const Mat *x, int k) {
    }

    void normalize(Mat *dest, const Mat *org) {
    }

    void findClosestCentroids(int *idx, const Mat *x, const Mat *centroids) {
    }

    void matAddRow(Mat *ans, int rdest, Mat *x, int rorg) {
        int n = x->ncols();
        if (ans->ncols() != n) throw "Matrix width mismatch exception";
        for (int i=0; i<n; i++) {
	    ans->get(rdest, i) += x->get(rorg, i);
        }
    }

    void computeCentroids(Mat *centroids, Mat* x, const int *idx, int k) {
        matSetZero(centroids);
        int howMany[k];
        memset(howMany, 0, k*sizeof(int));
        int n = x->nrows();
        for (int i=0; i<n; i++) {
	    matAddRow(centroids, idx[i], x, i);
	    howMany[idx[i]]++;
        }
        for (int i=0; i<k; i++) {
	    for (int j=0; j<x->ncols(); j++)
	        centroids->get(i,j) /= howMany[i];
        }
    }
}
