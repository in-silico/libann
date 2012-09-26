#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <map>
#include <algorithm>
#include <matrix.h>
#include "global.h"

using namespace std;

Config *c;

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
    str2mat(m, c->getVal("rgb2XYZ").c_str());
    m->transpose();
    matMult(dest, org, m);
    delete m;
}

void XYZ2rgb(Mat *dest, Mat *org) {
    Mat *m = new Mat(9);
    m->setSize(3,3);
    str2mat(m, c->getVal("rgb2XYZ").c_str());
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

void rgb2xy(Mat *dest, Mat *org) {
    int n = org->nrows();
    Mat *Yxy = new Mat(3*n);
    rgb2Yxy(Yxy,org);
    dest->setSize(n,2);
    rep(i,n) rep(j,2) dest->get(i,j) = org->get(i,j+1);
    delete Yxy;
}

void Yxy2rgb(Mat *dest, Mat *org) {
    int n = org->nrows();
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

void xy2rgb(Mat *dest, Mat *org) {
    double defY;
    sscanf(c->getVal("defY").c_str(),"%lf",&defY);
    int n = org->nrows();
    Mat *Yxy = new Mat(3*n);
    Yxy->setSize(n,3);
    rep(i,n) {
	Yxy->get(i,0) = defY;
	rep(j,2) Yxy->get(i,j+1) = org->get(i,j);
    }
    Yxy2rgb(dest,Yxy);
    delete Yxy;
}

int main(int argc, char **argv) {
    if (argc == 1) {
	printf("Usage: %s <mode> <inMat> <outMat>\n",argv[0]);
	printf("Modes: rgb2xy xy2rgb\n");
	return 1;
    }
    try {
	c = new Config("colorm.conf");
	int matSize = atof(c->getVal("matSize").c_str());
	Mat *m1 = new Mat(matSize);
	Mat *m2 = new Mat(matSize);
	m1->load(argv[2]);
	if ( strcmp(argv[1],"rgb2xy")==0 ) {
	    rgb2xy(m2,m1);
	} else if (strcmp(argv[1],"xy2rgb") == 0) {
	    xy2rgb(m2,m1);
	} else {
	    fprintf(stderr,"Not recognized model %s\n",argv[1]);
	    return 1;
	}
	m2->save(argv[3]);
	delete c; delete m1; delete m2;
    } catch (const char *s) {
	fprintf(stderr,"Exception: %s\n",s);
	return 2;
    }
    return 0;
}
