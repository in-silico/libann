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
    int n,m;
    double x;
    Mat *m1 = new Mat(10000);
    Mat *m2 = new Mat(10000);
    Mat *m3 = new Mat(10000);
    scanf("%d %d", &n, &m);
    m3->setSize(1,m);
    m2->setSize(1,m);
    m1->setSize(n,m);
    rep(i,n) {
	rep(j,m) {
    	    scanf("%lf",&x);
	    m1->get(i,j) = x;
	}
    }

    LibAnn::std(m3,m2,m1);
    printMat(m2);
    printMat(m3);
    } catch (const char *s) {
	printf("Excpetion %s\n",s);
    }
}
