#include "kmeans.h"
#include <set>
#include <limits>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#define rep(i,n) for(int i=0; i<(n); i++)
#define rep2(i,n) for(int i=0; i<=(n); i++)
#define repf(i,a,b) for(int i=(a); i<=(b); i++)
#define repb(i,a,b) for (int i=(a); i>=b; i--)

const double inf = std::numeric_limits<double>::max();

using namespace std;

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

    int kmeans(Mat *centers, Mat *x, Mat *initial_centroids, KMeansConf *conf) {
	int m = x->nrows(), n = x->ncols();
	int k = initial_centroids->nrows();
	if (n != initial_centroids->ncols()) throw "Matrix width mismatch excpetion";

	Mat *c = new Mat(k*n);
	int *idx; int i;
	int *pidx=0; int maxIter=100;
	if (conf != 0) {
	    pidx = conf->pidx;
	    maxIter = conf->maxIter;
	}
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

    int *tree;
    int N;

    int log2ceil(int x) {
	int ans=0;
	while (x>0) {
	    x = x>>1;
	    ans++;
	}
	return ans;
    }

    // add v to value at x
    void set(int x, int v) {
	while(x <= N) {
	    tree[x] += v;
	    x += (x & -x);
	}
    }

    // get cumulative sum up to and including x
    int get(int x) {
	int res = 0;
	while(x) {
	    res += tree[x];
	    x -= (x & -x);
	}
	return res;
    }

    // get largest value with cumulative sum less than or equal to x;
    // for smallest, pass x-1 and add 1 to result
    int getind(int x) {
	int idx = 0, mask = N;
	while(mask && idx < N) {
	    int t = idx + mask;
	    if(x >= tree[t]) {
		idx = t;
		x -= tree[t];
	    }
	    mask >>= 1;
	}
	return idx;
    }

    void randPerm(int *ans, int k, int n) {
	int ls = log2ceil(n);
	tree = new int[(1<<ls)+1];
	N = (1<<ls);
	rep(i,n) tree[i]=0;
	rep(i,n) set(i+1,1);
	srand(time(0));
	repb(i,n,1) {
	    int v = rand() % i;
	    int ix = getind(v) + 1;
	    set(ix,-1); //add -1 (set to zero)
	    ans[n-i] = ix;
	    if ((n-i)>=k) break;
	}
	delete tree;
    }

    /*void randPerm(int *ans, int k, int n) {
      ftree = new int[n+1];
      ftN = n;
      k = min(n,k);
      rep(i,n+1) ftree[i]=0;
      rep(i,n) setft(i+1,1);
      srand(time(0));
      repb(i,n,1) {
      int v = rand() % i;
      int ix = getindft(v) + 1;
      setft(ix,-1); //add -1 (set to zero)
      if ((n-i)>=k) break;
      ans[n-i] = ix-1;
      }
      delete ftree;
      }*/

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
