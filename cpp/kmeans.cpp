#include "kmeans.h"
#include <limits>
#include <cstring>
#include <cmath>
#define rep(i,n) for(int i=0; i<(n); i++)

const double inf = std::numeric_limits<double>::max();

/* Space for private helper functions here */


/* Public functions here */

namespace LibAnn {

    int kmeans(Mat *centers, Mat *x, Mat *initial_centroids, int maxIter) {
        int m = x->nrows(), n = x->ncols();
	    int k = initial_centroids->nrows();
	    if (n != initial_centroids->ncols()) throw "Matrix width mismatch excpetion";
	
	    Mat *c = new Mat(k*n);
	    int *idx = new int[m];
	    matCopy(c, initial_centroids);

	    for (int i=0; i<maxIter; i++) {
	        if (!findClosestCentroids(idx, x, c)) break;
	        computeCentroids(c, x, idx, k);
	    }
	    matCopy(centers,c);

	    delete c; delete idx;
    }

    void kmeansInit(Mat *centroids, Mat *x, int k) {
    }

    void std(Mat *stdev, Mat *mean, Mat *x){
        double tmp;
        int x_c = x->ncols(), x_r = x->nrows();
        //mean
        rep(j,x_c){
            tmp=0;
            rep(i,x_r){
                tmp += x->get(i,j);
            }
            mean->get(0,j)= tmp/(x_r);
        }
        //standar deviation
        rep(j,x_c){
            tmp=0;
            rep(i,x_r){
                tmp += pow(x->get(i,j),2);
            }
            stdev->get(0,j)= sqrt((tmp/x_r) - pow(mean->get(0,j),2));
        }

    }

    void normalize(Mat *dest, Mat *org) {
    }

    double distance(Mat *a, Mat *b,const int a_row,const int b_row){
        if(a->ncols()!=b->ncols())
            throw "The vectors have different dimensions";
        double acum=1;
        rep(j,a->ncols()){
            acum*= (a->get(a_row,j) - b->get(b_row,j) );
        }
        return acum;
    }

    bool findClosestCentroids(int *idx, Mat *x, Mat *centroids) {
        int x_r=x->nrows() , cent_r= centroids->nrows(), row_min=0;
        double m,tmp;
        bool ans = false;
        rep(i,x_r){
            m = inf;
            rep(j,cent_r){
                tmp = distance(x,centroids,i,j);
                if(tmp<m){
                    m=tmp;
                    row_min=j;
                }
            }
            ans |= (idx[i]!=row_min);
            idx[i]=row_min;
        }
        return ans;
    }

    void matAddRow(Mat *ans, int rdest, Mat *x, int rorg) {
        int n = x->ncols();
        if (ans->ncols() != n) throw "Matrix width mismatch exception";
        for (int i=0; i<n; i++) {
	    ans->get(rdest, i) += x->get(rorg, i);
        }
    }

    void computeCentroids(Mat *centroids, Mat* x, const int *idx, int k) {
	    centroids->setSize(k,x->ncols());
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
