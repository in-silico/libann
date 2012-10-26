#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <map>
#include <algorithm>
#include <matrix.h>
#include <highgui.h>
#include <cv.h>
#include <kmeans.h>

using namespace std;
using namespace LibAnn;

#define rep(i,n) for(int i=0; i<(n); i++)
#define rep2(i,n) for(int i=0; i<=(n); i++)
#define repf(i,a,b) for(int i=(a); i<=(b); i++)
#define repb(i,a,b) for (int i=(a); i>=b; i--)


void str2mat(Mat *ans, const char *src) {
    char buf[100];
    strcpy(buf, src);
    char *tok = strtok(buf,",;");
    for (int i=0; i<ans->nrows(); i++) {
	for (int j=0; j<ans->ncols(); j++) {
	    ans->get(i,j) = atof(tok);
	    tok = strtok(0,",;");
	}
    }
}

void rgb2XYZ(Mat *dest, Mat *org) {
    Mat *m = new Mat(9);
    m->setSize(3,3);
    str2mat(m,"2.76883,1.75171,1.13014;1.00000,4.59061,0.06007;0.00000,0.05651,5.59417");
    m->transpose();
    matMult(dest, org, m);
    delete m;
}

void XYZ2rgb(Mat *dest, Mat *org) {
    Mat *m = new Mat(9);
    m->setSize(3,3);
    str2mat(m, "2.76883,1.75171,1.13014;1.00000,4.59061,0.06007;0.00000,0.05651,5.59417");
    m->transpose();
    matInv(m,m);
    matMult(dest,org,m);
    delete m;
}

void rgb2Yxy(Mat *dest, Mat *org) {
    rgb2XYZ(dest,org);
    for (int i=0; i<dest->nrows(); i++) {
	double tmp[3];
	double sum=0;
	for (int j=0; j<3; j++) {
	    tmp[j] = dest->get(i,j);
	    sum += tmp[j];
	}
	dest->get(i,0) = tmp[1]; //Y
	dest->get(i,1) = tmp[0]/sum; //x
	dest->get(i,2) = tmp[1]/sum; //y
    }
}

void Yxy2rgb(Mat *dest, Mat *org) {
    int n = org->nrows();
    dest->setSize(n,3);
    rep (i,n) {
	double Y = org->get(i,0);
	double x = org->get(i,1);
	double y = org->get(i,2);
	double z = 1-x-y;
	double den = Y/y;
	dest->get(i,0) = den*x;
	dest->get(i,1) = Y;
	dest->get(i,2) = den*z;
    }
    XYZ2rgb(dest,dest);
}

struct Options {
    int w,h; //geometry
};

Options opt;

Mat* img2mat(const char *fname) {
    IplImage *img = cvLoadImage(fname);
    int n = opt.w = img->width; 
    int m = opt.h = img->height;
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

void mat2img(const char *fname, Mat *x, Options g) {
    int cnt=0;
    try {
	int *idx = new int[g.w*g.h];
	
	IplImage *im1 = cvCreateImage(cvSize(g.w,g.h), IPL_DEPTH_8U, 3);
	rep(i,g.h) {
	    uchar *row = (uchar*) (im1->imageData + i*im1->widthStep);
	    rep(j,g.w) {
		int color = i*g.w + j;
		rep(k,3)
		    row[j*im1->nChannels + k] = (x->get(color,k) > 255) ? 255 : (uchar)x->get(color,k);
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
    if (argc == 1) {
        printf("usage: %s <orgImage> <destImage> <yfactor>",argv[0]);
        return 1;
    }
    Mat *org = img2mat(argv[1]);
    double fac = atof(argv[3]);
    Mat *tmp = new Mat(org->maxdata);
    rgb2Yxy(tmp,org);
    rep(i,tmp->nrows()) {
        tmp->get(i,0) = fac*tmp->get(i,0);
    }
    Yxy2rgb(org,tmp);
    mat2img(argv[2],org,opt);
    delete org; delete tmp;
}
