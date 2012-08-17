#include <highgui.h>
#include <cv.h>
#include "../matrix.cpp"
#include "../kmeans.cpp"

#define K 16

using namespace LibAnn;

int imgw,imgh;

void printMat(Mat *x) {
    rep(i,x->nrows()) {
	rep(j,x->ncols())
	    printf("%.2lf\t",x->get(i,j));
	printf("\n");
    }
    printf("\n");
}

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

void readMat(Mat *ans, const char *fname, int w, int h) {
    FILE *f = fopen(fname,"r");
    double x;
    ans->setSize(w,h);
    rep(i,w) {
	rep(j,h) {
	    fscanf(f,"%lf",&x);
	    ans->get(i,j) = x;
	}
    }    
    fclose(f);
}

void test1() {
    Mat *m1 = new Mat(1000000);//getMatrix();
    readMat(m1,"img.txt",64*48,3);
    Mat *m2 = new Mat(100);
    readMat(m2,"c.txt",K,3);
    try {
	kmeans(m2,m1,m2,10);
	printMat(m2);
    } catch (const char *s) {
	printf("Exception: %s\n",s);
    }
    delete m1; delete m2;
}

void test2(const char *fname) {
    int cnt=0;
    try {
	Mat *m1 = getMatrix(fname);
	Mat *c = new Mat(300);
	kmeansInit(c,m1,K);
	int *idx = new int[m1->nrows()];
	kmeans(c,m1,c,10,idx);
	printMat(c);

	IplImage *im1 = cvCreateImage(cvSize(imgw,imgh), IPL_DEPTH_8U, 3);
	//int cnt=0;
	rep(i,imgh) {
	    uchar *row = (uchar*) (im1->imageData + i*im1->widthStep);
	    rep(j,imgw) {
		int color = idx[cnt++];
		rep(k,3)
		    row[j*im1->nChannels + k] = c->get(color,k);
	    }
	}
	cvSaveImage("out.png",im1);
	cvReleaseImage(&im1);
	delete idx; delete m1; delete c;
    } catch (const char *s) {
	printf("Exception: %s\n",s);
	printf("%d\n",cnt);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) printf("Image name expected\n");
    else test2(argv[1]);
}
