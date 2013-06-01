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
void correspTest(Image<T> &ans, Image<T> &img1, Image<T> &img2) {
    corresp pts; CorrespParams params;
    params.matchThresh=-20;
    params.nmsThresh=1e8;
    findCorrespondences(pts, img1, img2, params);
    for (int i=0; i<pts.size(); i++) {
        ipt p1 = pts[i].first, p2 = pts[i].second;
        printf("(%d, %d) -> (%d, %d)\n",p1.x,p1.y,p2.x,p2.y);
    }
    printf("Done\n");
}

void test1() {
    IplImage *cvi = cvLoadImage("test.jpg");
    int m = cvi->width, n = cvi->height;
    IplImage *cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    cvCvtColor(cvi,cvg,CV_RGB2GRAY);
    Image<float> img1(n,m);
    Image<float> img2(n,m);
    Image<float> ans(n,m);
    cvimg2img(img1, cvg); cvimg2img(img2, cvg);
    
    cornerTest(ans, img1);
    correspTest(ans,img1,img2);
    saveImg("test2.jpg", ans);
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
