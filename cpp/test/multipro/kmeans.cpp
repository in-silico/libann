#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>

using namespace std;

#define rep(i,n) for(int i=0; i<(n); i++)
#define rep2(i,n) for(int i=0; i<=(n); i++)
#define repf(i,a,b) for(int i=(a); i<=(b); i++)
#define repb(i,a,b) for (int i=(a); i>=b; i--)

#include <algorithm>
#include <map>
#include <kmeans.h>
#include "global.h"

using namespace LibAnn;

int K,ntimes,sampleSize;//int MaxIter
int matsize;
KMeansConf conf;
Mat *bigmat;

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
	matsize = 10000;
	bigmat = new Mat(matsize);
	bigmat->setSize(0,3);
	if (argc < 2) {
	    printf("Usage: %s (-K <# of clusters>) (-I <max Iter>) (-T <# of times>) (-N <# of threads>) <imgmat>\n", argv[0]);
	    return 1;
	}
	argc--; argv++;
	rep(i,argc) {
	    if (strcmp(argv[i],"-K")==0) sscanf(argv[++i],"%d",&K);
	    else if (strcmp(argv[i],"-I")==0) sscanf(argv[++i],"%d",&(conf.maxIter));
	    else if (strcmp(argv[i],"-T")==0) sscanf(argv[++i],"%d",&ntimes);
	    else if (strcmp(argv[i],"-N")==0) sscanf(argv[++i],"%d",&(conf.nthreads));
	    else bigmat->load(argv[i]);
	}
	time( &start );
	runkm();        
	time( &end );
	printf("Kmeans running time: %.1lf s\n", difftime(end,start));
    } catch (const char *s) {
	printf("Exception: %s\n",s);
    }
    return 0;
}
