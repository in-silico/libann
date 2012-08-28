#include <set>
#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <map>
#include <highgui.h>
#include <cv.h>

#define rep(i,n) for(int i=0; i<(n); i++)
#define rep2(i,n) for(int i=0; i<=(n); i++)
#define repf(i,a,b) for(int i=(a); i<=(b); i++)
#define repb(i,a,b) for (int i=(a); i>=b; i--)

using namespace std;

#define TRANSP 1

class Mat {
    int r,c;    
public:
    int maxdata;
    double *data;
    char flags;

    Mat(int maxdata) {
	this->maxdata = maxdata;
	this->data = new double[maxdata];
	flags = 0;
	r=c=0;
    }

    ~Mat() {
	delete [] data;
    }

    double& get(int i, int j) {
	int ni=i,nj=j;
	if (flags & TRANSP) {
	    ni=j; nj=i;
	}
	if (ni>=0 && ni<r && nj>=0 && nj<c) return data[ni*c + nj];
	throw "Out of bounds index exception";
    }

    void transpose() { flags ^= TRANSP; }

    void setSize(int w, int h);

    int nrows() { return (flags & TRANSP) ? c : r; }
    int ncols() { return (flags & TRANSP) ? r : c; }
    void save(const char *fname);
    void load(const char *fname);
};

struct MatFileHead {
    int r,c;
    char flags;
};

void matCopy(Mat *ans, Mat *src) {
    ans->setSize(src->nrows(), src->ncols());
    rep(i,ans->nrows())
	rep(j,ans->ncols())
	    ans->get(i,j) = src->get(i,j);
}

void matCopyRow(Mat *ans, int rans, Mat *src, int rsrc) {
    if (ans->ncols() != src->ncols()) throw "Matrix width mismatch exception";
    int m = src->ncols();
    rep(i,m)
	ans->get(rans,i) = src->get(rsrc, i);
}

void Mat::save(const char *fname) {
    FILE *f = fopen(fname, "wb");
    MatFileHead h;
    h.r = r; h.c = c; h.flags = flags;
    fwrite(&h,sizeof(h),1,f);
    fwrite(data,sizeof(double),r*c,f);
    fclose(f);
}

void Mat::load(const char *fname) {
    FILE *f = fopen(fname, "rb");
    MatFileHead h;
    if (fread(&h,sizeof(h),1,f) != 1) throw "Wrong matrix format exception";
    setSize(h.r,h.c); flags = h.flags;
    if (fread(data,sizeof(double),r*c,f) != (unsigned int)(r*c)) 
	throw "Wrong matrix format exception";
    fclose(f);
}

void Mat::setSize(int w, int h) { 
    if (w*h > maxdata) throw "Insufficient space for this matrix exception";
    r=w; c=h; flags=0; 
}

void matSetZero(Mat *ans) {
    rep(i,ans->nrows()) 
	rep(j,ans->ncols())
	    ans->get(i,j)=0;
}

    const double inf = std::numeric_limits<double>::max();

    struct KMeansConf {
	int maxIter;
	int *pidx;
	int nthreads;

	KMeansConf() {
	    maxIter=100;
	    pidx=0;
	    nthreads=1;
	}
    };

    /**
     * Runs the k-means algorithm over the given data matrix x (Each column is a instance)
     * and returns a k column vector with the chosen centers. It iterates until each data instance does
     * not change of center or for maxIter iterations.
     */
    int kmeans(Mat *centers, Mat *x, Mat *initial_centroids, KMeansConf *conf=0);
     
    /**
     * Compute the mean squared error of projecting x on centers
     */
    double kmeansError(Mat *centers, Mat *x);

    /**
     * Creates a k-dimensional vector centroids picking randomly from x a data point for each centroid and
     * making sure that no data point is chosen twice.
     */
    void kmeansInit(Mat *centroids, Mat *x, int k);

    /**
     * Creates a random permutation of length k out of n elements
     */
    void randPerm(int *ans, int k, int n);

    /**
     * Computes the standar deviation and mean vector of each column of x into ans
     */
    void stdv(Mat *stdev, Mat *mean, Mat *x);

    /**
     * Normalices each column of org to have zero mean and unit standard deviation, and stores the
     * result to dest. The mean and vector of standard deviations is returned as a row vector.
     */
    void normalize(Mat *dest, Mat *mean, Mat *stdev, Mat *org);
    
    /**
     * Finds the closest centroids to each instance of x in idx. Meaning that idx[j] is a number
     * between 0 and k-1 indicating which centroid (row vector) is closer to x[j]. The vector idx
     * is a integer vector of size greater or equal to the number of rows of X.
     */
    bool findClosestCentroids(int *idx, Mat *x, Mat *centroids);

    /**
     * Computes each centroid j as the mean of the data instances whose idx[i]==j.
     */
    void computeCentroids(Mat *centroids, Mat* x, const int *idx, int k);


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

// Begin of testing code

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
