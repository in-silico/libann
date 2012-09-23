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

void readParams() {
}

int main() {
    c = new Config("colorm.conf");
    readParams();
    delete c;
}
