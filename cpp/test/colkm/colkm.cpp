#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <map>
#include <kmeans.h>
#include <highgui.h>
#include <cv.h>

#define rep(i,n) for(int i=0; i<(n); i++)
#define rep2(i,n) for(int i=0; i<=(n); i++)
#define repf(i,a,b) for(int i=(a); i<=(b); i++)
#define repb(i,a,b) for (int i=(a); i>=b; i--)

using namespace std;
using namespace LibAnn;

int K,ntimes,sampleSize;//int MaxIter
Mat *bigmat;
KMeansConf conf;

void printMat(Mat *x) {
    rep(i,x->nrows()) {
	rep(j,x->ncols())
	    printf("%.2lf\t",x->get(i,j));
	printf("\n");
    }
    printf("\n");
}

void appendMatrix(const char *fname) {
    IplImage *img = cvLoadImage(fname);
    if (img == 0) {
	fprintf(stderr,"Image %s does not exist\n",fname);
	return;
    }
    cvSmooth(img,img);
    int n = img->width, m = img->height;
    int offset = bigmat->nrows();
    bigmat->setSize(offset + n*m ,3);
    rep(i,m) {
	uchar *row = (uchar*)(img->imageData + i*img->widthStep);
	rep(j,n) {
	    rep(k,3) {
		uchar x = row[j*img->nChannels + k];
		bigmat->get(offset + i*n+j,k) = x;
	    }
	}
    }
    cvReleaseImage(&img);
}

int cmpInt(const void *a, const void *b) {
    return (*((const int*)a) - *((const int*)b));
}

void takeSample(Mat *ans, Mat *x) {
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

void runkm( ) {
    int cnt=0;
    try {
	Mat *c = new Mat(3*K);
        Mat *tmp = new Mat(3*K);
	kmeansInit(c,bigmat,K);
	int *idx = new int[bigmat->nrows()];
        double best = 1e100, current;
        rep(i,ntimes) {
	    kmeansInit(tmp,bigmat,K);

            //kmeans(tmp,bigmat,tmp,MaxIter,2,idx);
	    kmeans(tmp,bigmat,tmp,&conf);

            current = kmeansError(tmp,bigmat);
            if (current < best) {
                best = current;
                matCopy(c,tmp);
            }
        }
	printMat(c);
	c->save("km.mat");
	delete idx; delete c;
    } catch (const char *s) {
	printf("Exception: %s\n",s);
	printf("%d\n",cnt);
    }
}

int main(int argc, char **argv) {
    try {
        time_t start, end;
        K = 16; //Default value of K
        //MaxIter = 200; //Default of maxIter
        ntimes = 1; //Default of ntimes
	sampleSize = -1; //Take the complete set as a sample
        bigmat = new Mat(100000000);
        bigmat->setSize(0,3);
        if (argc < 2) {
	    printf("Usage: %s (-K <# of clusters>) (-I <max Iter>) (-T <# of times>) (-N <# of threads>) (-S <sampleSize>) <image1> (<image_i>*)\n", argv[0]);
	    return 1;
        }
        argc--; argv++;
        rep(i,argc) {
	    if (strcmp(argv[i],"-K")==0) sscanf(argv[++i],"%d",&K);
	    else if (strcmp(argv[i],"-I")==0) sscanf(argv[++i],"%d",&(conf.maxIter));
            else if (strcmp(argv[i],"-T")==0) sscanf(argv[++i],"%d",&ntimes);
	    else if (strcmp(argv[i],"-S")==0) sscanf(argv[++i],"%d",&sampleSize);
	    else if (strcmp(argv[i],"-N")==0) sscanf(argv[++i],"%d",&(conf.nthreads));
	    else appendMatrix(argv[i]);
        }
	takeSample(bigmat, bigmat);
        time( &start );
        runkm();        
        time( &end );
        printf("Kmeans running time: %.1lf s\n", difftime(end,start));
    } catch (const char *s) {
        printf("Exception: %s\n",s);
    }
    return 0;
}
