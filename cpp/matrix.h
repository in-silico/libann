#ifndef MATRIX_H
#define MATRIX_H

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

    void setSize(int w, int h) { r=w; c=h; }

    int nrows() { return (flags & TRANSP) ? c : r; }
    int ncols() { return (flags & TRANSP) ? r : c; }
};

void matCopy(Mat *ans, Mat *src);
void matMult(Mat *ans, Mat *a, Mat *b);
bool matInv(Mat *ans, Mat *x);
bool matPinv(Mat *ans, Mat *x);
void matSetZero(Mat *ans);

#endif
