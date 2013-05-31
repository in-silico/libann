/*
 * =====================================================================================
 *
 *       Filename:  img_test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/24/2013 03:22:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sebastian Gomez (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <cstdio>
#include <iostream>
#include "img_lib.h"

using namespace std;

/*template<class T>
Image<T>* loadImg(const char *s) {
    IplImage *cvi = cvLoadImage("test.jpg");
    int m = cvi->width, n = cvi->height;
    IplImage *cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    cvCvtColor(cvi,cvg,CV_RGB2GRAY);
    Image<T>* ans = new Image<T>(n,m);
    cvimg2img(ans, cvg);
    return ans;
}*/

template<class T>
void saveImg(const char *s, Image<T> &img) {
    int m = img.cols, n = img.rows;
    IplImage *cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    img2cvimg(cvg, img);
    cvSaveImage(s,cvg);
}

template <class T>
void negative(Image<T> &img2, Image<T> &img) {
    for (int i=0; i<img.rows; i++) {
        for (int j=0; j<img.cols; j++) {
            img2(i,j) = 255 - img(i,j);
        }
    }
}


template <class T>
void testFilter(Image<T> &img2, Image<T> &img) {
    gaussianFilter(img2, img, 2.0);
}

template<class T>
void drawCorner(Image<T> &img, Point2D<int> &p, int w=5) {
    int wh = w/2;
    for (int i=-wh; i<=wh; i++) {
        for (int j=-wh; j<=wh; j++) {
            if (i==j || i==-j) {
                int nr = p.x+i, nc = p.y+j;
                if (nr>=0 && nr<img.rows && nc>=0 && nc<img.cols) img(nr,nc)=0; //set corner to black
            }
        }
    }
}

template <class T>
void cornerTest(Image<T> &img2, Image<T> &img) {
    Image<T> tmp(img.rows, img.cols);
    gaussianFilter(img2, img, 2.0);
    harrisFilter(tmp, img2);
    img2 = img;
    vector< Point2D<int> > corners;
    tmp.nonMaxSupr(7,1E6,corners);
    int n = corners.size();
    printf("%d\n",n);
    for (int i=0; i<n; i++) {
        drawCorner(img2, corners[i]);
    }
}

void test1() {
    IplImage *cvi = cvLoadImage("test.jpg");
    int m = cvi->width, n = cvi->height;
    IplImage *cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    cvCvtColor(cvi,cvg,CV_RGB2GRAY);
    Image<float> img(n,m);
    Image<float> img2(n,m);
    cvimg2img(img, cvg);
    
    cornerTest(img2, img);    
    saveImg("test2.jpg", img2);
    cvReleaseImage( &cvi ); cvReleaseImage( &cvg );
}

void test2(){
    IplImage *cvi = cvLoadImage("test.jpg");
    int m = cvi->width, n = cvi->height;
    IplImage *cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    cvCvtColor(cvi,cvg,CV_RGB2GRAY); 
    Image<float> img(n,m);
    Image<float> dest(n,m);
    cvimg2img(img, cvg);
    Image<float> dx(n,m);
    Image<float> dy(n,m);
    harrisFilter(dest, img);
    /*Image<float> r(n,m);
    Image<float> t(n,m);
    cart2pol(r, t, dx, dy);
    r.normalize(255);*/
    dest.normalize(255);
    saveImg("test2.jpg", dest);
}

int main() {
    try{
        test1();
        //test2();    
    } catch(ImgError e) {
        printf("error %d: %s\n", e.code, e.message.c_str());
    }
}
