#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <map>
#include <kmeans.h>
#include <highgui.h>

using namespace std;
using namespace LibAnn;

#define rep(i,n) for(int i=0; i<(n); i++)
#define rep2(i,n) for(int i=0; i<=(n); i++)
#define repf(i,a,b) for(int i=(a); i<=(b); i++)
#define repb(i,a,b) for (int i=(a); i>=b; i--)

Mat *c;
int imgw,imgh;

Mat* getMatrix(const char *fname) {
    IplImage *img = cvLoadImage(fname);
    int n = img->width, m = img->height;
    imgw=n; imgh=m;
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

void applyColor(const char* fname, Mat *x) {
    int cnt=0;
    try {
	int *idx = new int[imgw*imgh];
	findClosestCentroids(idx, x, c);

	IplImage *im1 = cvCreateImage(cvSize(imgw,imgh), IPL_DEPTH_8U, 3);
	rep(i,imgh) {
	    uchar *row = (uchar*) (im1->imageData + i*im1->widthStep);
	    rep(j,imgw) {
		int color = idx[cnt++];
		rep(k,3)
		    row[j*im1->nChannels + k] = c->get(color,k);
	    }
	}
	cvSaveImage(fname,im1);
	cvReleaseImage(&im1);

	delete idx;
    } catch (const char *s) {
	printf("Exception: %s\n",s);
	printf("%d\n",cnt);
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
	printf("Usage: %s <orgImg> <destImg>\n", argv[0]);
	return 1;
    }

    try {
	c = new Mat(500);
	c->load("km.mat");
	Mat *x = getMatrix(argv[1]);
	applyColor(argv[2], x);
	delete c; delete x;
    } catch (const char *s) {
	printf("Exception: %s\n",s);
	return 2;
    }
    return 0;
}

