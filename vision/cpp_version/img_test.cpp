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
    tmp.nonMaxSupr(7,1E8,corners);
    int n = corners.size();
    printf("%d\n",n);
    for (int i=0; i<n; i++) {
        drawCorner(img2, corners[i]);
    }
}

template<class T>
void correspTest(Image<T> &img1, Image<T> &img2) {
    corresp pts; CorrespParams params;
    params.matchThresh=-200000;
    params.nmsThresh=1e10;
    params.scoreW = 11;
    findCorrespondences(pts, img1, img2, params);

    //create image with two images left and right
    int n = img1.rows; //assumed equal to img2.rows
    int m = img1.cols + img2.cols + 1;
    IplImage *cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    for (int i=0; i<n; i++) {
        uchar *row = (uchar*)(cvg->imageData + i*cvg->widthStep);
        T *r1 = img1.getRow(i); T *r2 = img2.getRow(i);
        for (int j=0; j<img1.cols; j++) row[j] = (uchar)r1[j];
        row[img1.cols] = 0; //black line in the middle
        for (int j=0; j<img2.cols; j++) row[img1.cols + j + 1] = (uchar)r2[j];
    }

    double p = 0.2; //probability that a correspondence is shown
    for (int i=0; i<pts.size(); i++) {
        double r = (rand() / ((double)RAND_MAX));
        ipt p1 = pts[i].first, p2 = pts[i].second;
        if (r<p) {
            cvCircle(cvg,cvPoint(p1.y,p1.x),3,cvScalar(0,0,0));
            cvCircle(cvg,cvPoint(p2.y+img1.cols+1,p2.x),3,cvScalar(0,0,0));
            cvLine(cvg,cvPoint(p1.y,p1.x),cvPoint(p2.y +img1.cols + 1,p2.x),cvScalar(0,0,0));
        }
        printf("(%d, %d) -> (%d, %d)\n",p1.x,p1.y,p2.x,p2.y);
    }
    cvSaveImage("out.jpg",cvg);
    printf("Done\n");
}

void testFilter() {
    IplImage *cvi = cvLoadImage("test.jpg");
    int m = cvi->width, n = cvi->height;
    IplImage *cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    cvCvtColor(cvi,cvg,CV_RGB2GRAY);
    Image<float> img1(n,m); cvimg2img(img1, cvg); 
    Image<float> img2(n,m);
    testFilter(img2, img1);

    //Image<float> ans(n,2*m);    
    //cornerTest(ans, img1);
    //correspTest(img1,img2);
    saveImg("out.jpg", img2);
    cvReleaseImage( &cvi ); cvReleaseImage( &cvg );
}

void testHarris() {
    IplImage *cvi = cvLoadImage("test.jpg");
    int m = cvi->width, n = cvi->height;
    IplImage *cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    cvCvtColor(cvi,cvg,CV_RGB2GRAY);
    Image<float> img1(n,m); cvimg2img(img1, cvg); 
    Image<float> img2(n,m);
    //testFilter(img2, img1);

    //Image<float> ans(n,2*m);    
    cornerTest(img2, img1);
    //correspTest(img1,img2);
    saveImg("out.jpg", img2);
    cvReleaseImage( &cvi ); cvReleaseImage( &cvg );

}

void test3() {
    IplImage *cvi = cvLoadImage("left.jpg");
    int m = cvi->width, n = cvi->height;
    IplImage *cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    cvCvtColor(cvi,cvg,CV_RGB2GRAY);
    Image<float> img1(n,m); cvimg2img(img1, cvg); 

    cvi = cvLoadImage("right.jpg");
    m = cvi->width, n = cvi->height;
    cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    cvCvtColor(cvi,cvg,CV_RGB2GRAY);
    Image<float> img2(n,m); cvimg2img(img2, cvg);

    //Image<float> ans(n,2*m);    
    //cornerTest(ans, img1);
    correspTest(img1,img2);
    //saveImg("test2.jpg", ans);
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
    srand( time(0) );
    try{
        //test1();
        testHarris();    
    } catch(ImgError e) {
        printf("error %d: %s\n", e.code, e.message.c_str());
    }
}
