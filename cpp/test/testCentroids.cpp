#include "../matrix.cpp"
#include "../kmeans.cpp"

void printMat(Mat *x) {
    rep(i,x->nrows()) {
	rep(j,x->ncols())
	    printf("%.2lf\t",x->get(i,j));
	printf("\n");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    try {
        int k,n,m;
        double x;
        Mat *m1 = new Mat(10000);
        Mat *m2 = new Mat(10000);
        int idx[1000];

        scanf("%d",&k);
        scanf("%d %d", &n, &m);
        m1->setSize(n,m);
        m2->setSize(k,m);
        rep(i,n) {
	        rep(j,m) {
            	    scanf("%lf",&x);
	            m1->get(i,j) = x;
	        }
        }
         rep(i,k) {
	        rep(j,m) {
            	    scanf("%lf",&x);
	            m2->get(i,j) = x;
	        }
        }
        
        LibAnn::findClosestCentroids(idx,m1,m2);
        
        rep(i,n)
            printf("%d ",idx[i]);
        printf("\n");
    } catch (const char *s) {
	    printf("Excpetion %s\n",s);
    }
}
