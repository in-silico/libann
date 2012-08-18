#include "kmeans.h"
#include <set>
#include <limits>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>
#define rep(i,n) for(int i=0; i<(n); i++)

const double inf = std::numeric_limits<double>::max();

namespace LibAnn {

    /* Space for private helper functions here */

    double distance(Mat *a, Mat *b,const int a_row,const int b_row){
        if(a->ncols()!=b->ncols())
            throw "The vectors have different dimensions";
        double acum = 0, tmp;
        rep(j,a->ncols()) {
	    tmp = ( a->get(a_row,j) - b->get(b_row,j) );
            acum += tmp*tmp; 
        }
        return acum;
    }

    void matAddRow(Mat *ans, int rdest, Mat *x, int rorg) {
        int n = x->ncols();
        if (ans->ncols() != n) throw "Matrix width mismatch exception";
        for (int i=0; i<n; i++) {
	    ans->get(rdest, i) += x->get(rorg, i);
        }
    }

    /* Public functions here */

    int kmeans(Mat *centers, Mat *x, Mat *initial_centroids, int maxIter, int *pidx) {
        int m = x->nrows(), n = x->ncols();
	    int k = initial_centroids->nrows();
	    if (n != initial_centroids->ncols()) throw "Matrix width mismatch excpetion";
	
	    Mat *c = new Mat(k*n);
	    int *idx; int i;
	    if (pidx==0) idx = new int[m];
	    else idx = pidx;
	    matCopy(c, initial_centroids);

	    for (i=0; i<maxIter; i++) {
	        if (!findClosestCentroids(idx, x, c)) break;
	        computeCentroids(c, x, idx, k);
	    }
	    matCopy(centers,c);

	    if (pidx == 0) delete idx;
	    delete c; 
	    return i;
    }

    double kmeansError(Mat *centers, Mat *x) {
	int n = x->nrows();
	int *idx = new int[n];
	findClosestCentroids(idx,x,centers);
	double ans = 0;
	rep(i, n) {
	    ans += distance(centers, x, idx[i], i);
	}
	return ans/n;
    }

    void copyRow(Mat* dest, Mat *ori, int r_dest,int r_ori){
    	if(dest->ncols()!=ori->ncols()) throw "Different dimentions (clumns).";
	rep(i,dest->ncols())
		dest->get(r_dest,i) = ori->get(r_ori,i);
    }

    void randPerm(int *ans, int k, int n) {	
	rep(i,k) {
	    int x; bool used;
	    do {
		x = rand()%n;
		used=false;
		rep(j,i) if (x==ans[j]) used=true;
	    } while (used);
	    ans[i]=x;
	}
    }

    void kmeansInit(Mat *centroids, Mat *x, int k) {
	int rp[k];
	srand(time(0));
	randPerm(rp, k, x->nrows());
	centroids->setSize(k,x->ncols());
	rep(i,k) {
	    copyRow(centroids, x, i, rp[i]);
	}	   
    }

    void stdv(Mat *stdev, Mat *mean, Mat *x){
        double tmp,tmp2;
        int x_c = x->ncols(), x_r = x->nrows();
        rep(j,x_c){
            tmp=tmp2=0;
            rep(i,x_r){
                tmp += x->get(i,j);
                tmp2 += pow(x->get(i,j),2);
            }
            mean->get(0,j)= tmp/(x_r);
            stdev->get(0,j)= sqrt((tmp2/x_r) - pow(mean->get(0,j),2));
        }
    }

    void normalize(Mat *dest, Mat *org) {
	Mat *stdev = new Mat(org->ncols());
	Mat *mean = new Mat(org->ncols());
	stdv(stdev, mean, org);
	int n = org->nrows(), m = org->ncols();
	dest->setSize(n, m);
	rep(i,n) {
	    rep(j,m) {
		dest->get(i,j) = (org->get(i,j) - mean->get(0,j)) / stdev->get(0,j);
	    }
	}
	delete stdev; delete mean;
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
