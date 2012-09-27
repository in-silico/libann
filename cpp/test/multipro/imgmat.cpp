#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <highgui.h>
#include <cv.h>
#include <kmeans.h>
#include "global.h"

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

struct Options {
    int w,h; //geometry
    char inverse; //is inverse operation
    int sample; //sample size
    char matName[50]; //matrix filename
};

Options opt;

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
		    row[j*im1->nChannels + k] = x->get(color,k);
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

int readOptions(int argc, char **argv) {
    int i=0;
    //default params
    opt.inverse=0;
    opt.sample=0;
    while (i<argc && argv[i][0]=='-') {
	switch(argv[i][1]) {
	    case 'I':
		sscanf(argv[++i],"%d",&(opt.w));
		sscanf(argv[++i],"%d",&(opt.h));
		opt.inverse=1;
		break;
	    case 'M':
		strcpy(opt.matName, argv[++i]);
		break;
	    case 'S':
		sscanf(argv[++i],"%d",&(opt.sample));
		break;
	    default:
		fprintf(stderr,"Error: Parametro %s no reconocido\n",argv[i]);
		throw "Unrecognized param exception";
	}
	i++;
    }
    return i;
}

void doInverse(const char *imgname) {
    Mat *m = new Mat(10000000);
    m->load(opt.matName);
    mat2img(imgname,m,opt);
    delete m;
}

void takeSample(Mat *ans, Mat *x, int sampleSize) {
    if (sampleSize <= 0) {
        matCopy(ans,x);
        return;
    }
    int *idx = new int[sampleSize];
    Mat *tmp = new Mat(sampleSize * x->ncols());
    randPerm(idx, sampleSize, x->nrows());
    tmp->setSize(sampleSize, x->ncols());
    rep(i,sampleSize) {
	matCopyRow(tmp,i,x,idx[i]);
    }
    matCopy(ans, tmp);

    delete []idx; delete tmp;
}

void doImg2Mat(char **imgnames, int imgc) {
    Mat *bigmat = new Mat(100000000);
    bigmat->setSize(0,3);
    rep (i,imgc) {
	Mat *tmp = img2mat(imgnames[i]);
	appendMat(bigmat,tmp);
	delete tmp;
    }
    takeSample(bigmat,bigmat,opt.sample);
    bigmat->save(opt.matName);
}

int main(int argc, char **argv) {
    if (argc == 1) {
	printf("Usage: %s (<options>) <image>*\n",argv[0]);
	return 1;
    }
    try {
	argc--; argv++;
	int tmp = readOptions(argc,argv);
	if (opt.inverse) {
	    doInverse(argv[tmp]);
	} else {
	    doImg2Mat(argv+tmp,argc-tmp);
	}
	return 0;
    } catch (const char *s) {
	fprintf(stderr,"Exception: %s\n",s);
	return 1;
    }
}
