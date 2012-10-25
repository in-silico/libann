#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <highgui.h>
#include <cv.h>
#include <kmeans.h>

#define rep(i,n) for(int i=0; i<(n); i++)
#define rep2(i,n) for(int i=0; i<=(n); i++)
#define repf(i,a,b) for(int i=(a); i<=(b); i++)
#define repb(i,a,b) for (int i=(a); i>=b; i--)

using namespace std;
using namespace LibAnn;

Mat* img2mat(const char *fname) {
    IplImage *img = cvLoadImage(fname);
    int n = img->width, m = img->height;
    Mat *mat = new Mat(3*n*m);
    mat->setSize(n*m,3);
    rep(i,m) {
	uchar *row = (uchar*)(img->imageData + i*img->widthStep);
	rep(j,n) {
	    rep(k,3) {
		uchar x = row[j*img->nChannels + k];
		mat->get(i*n+j,k) = x;
	    }
	}
    }
    cvReleaseImage(&img);
    return mat;
}

void normalize(double *vec, int k) {
    double sum=0;
    rep(i,k) sum += vec[i];
    rep(i,k) vec[i] /= sum;
}

void getFeatures(double *vec, Mat *ori, Mat *centroids) {
    int n = ori->nrows();
    int *idx = new int[n];
    int k = centroids->nrows();
    findClosestCentroids(idx,ori,centroids);
    rep(i,k) vec[i] = 0;
    rep(i,n) vec[idx[i]]++;
    normalize(vec,k);
    delete idx;
}

//Program Itself
int tag;

void doOutput(double *tmp, int k) {
    printf("%d",tag);
    rep(i,k) {
	printf(",%lf",tmp[i]);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    tag=0; //default tag
    if (argc==1) {
	printf("Pass as parameter the images you want to extract features from\n");
	printf("<-t tagNum> to add a tag number before data\n");
	return 1;
    }
    argc--; argv++;
    try {
	Mat *centers = new Mat(10000);
	centers->load("km.mat");
	int k = centers->nrows();
	double tmp[k];
	rep(i,argc) {
	    if (argv[i][0]=='-') {
		switch(argv[i][1]) {
		    case 't':
			//tag
			sscanf(argv[++i],"%d",&tag);
			break;
		}
	    } else {
		Mat *m = img2mat(argv[i]);
		getFeatures(tmp,m,centers);
		doOutput(tmp,k);
		delete m;
	    }
	}
    } catch (const char *s) {
	printf("Exception: %s\n", s);
    }
}
