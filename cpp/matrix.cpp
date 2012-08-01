#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <map>

using namespace std;

#define TRANSP 1
#define rep(i,n) for(int i=0; i<(n); i++)
#define repf(i,a,b) for (int i=(a); i<=(b); i++)
#define repb(i,a,b) for(int i=(a); i>=(b); i--)

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
	if (ni>0 && ni<r && nj>0 && nj<c) return data[i*c + j];
	throw "Out of bounds index exception";
    }

    void transpose() { flags ^= TRANSP; }

    int nrows() { return (flags & TRANSP) ? c : r; }
    int ncols() { return (flags & TRANSP) ? r : c; }
};

void matMult(Mat *ans, Mat *a, Mat *b) {
    int nr = a->nrows(), nc = b->ncols();
    if (nr*nc > ans->maxdata) throw "Matrix multiplication overflow";
    if (a->ncols() != b->nrows()) throw "Wrong dimensions in matrix multiplication";
    int nk = a->ncols();
    rep(i,nr) {
	rep(j,nc) {
	    double x = 0;
	    rep(k,nk) x += a->get(i,k)*b->get(k,j);
	    ans->get(i,j) = x;
	}
    }
}

int main(int argc, char **argv) {

}

