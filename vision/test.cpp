/*
 * =====================================================================================
 *
 *       Filename:  perspective.cpp
 *
 *    Description: Testing filter program 
 *
 *        Version:  1.0
 *        Created:  03/20/2013 12:08:55 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Sebastian Gomez (), 
 *   Organization:  UTP
 *
 * =====================================================================================
 */

#include "filter.cpp"
#include <cstdio>
#define NTMP 5

void img2mat(CvMat *mat, IplImage *img, int ch) {
    int n = img->height, m = img->width, nch = img->nChannels;
    rep(i,n) {
        uchar *orow = (uchar*)(img->imageData + i*img->widthStep);
        pix *drow = (pix*)(mat->data.ptr + i*mat->step);
        rep(j,m) {
            drow[j] = orow[j*nch + ch];
        }
    }
}

void mat2img(IplImage *img, CvMat* mat, int ch) {
    int n = mat->height, m = mat->width, nch = img->nChannels;
    rep(i,n) {
        uchar *drow = (uchar*)(img->imageData + i*img->widthStep);
        pix *orow = (pix*)(mat->data.ptr + i*mat->step);
        rep(j,m) {
            pix x = orow[j];
            if (x<0) x=0; if(x>255) x=255;
            drow[j*nch + ch] = x;
        }
    }
}


void test1() {
    CvMat *gk = cvCreateMat(5,5,CV_32F);
    IplImage *img = cvLoadImage("test.jpg");
    int m = img->width, n = img->height;
    CvMat *tmp[NTMP];
    rep(i,NTMP) tmp[i] = cvCreateMat(n,m,CV_32F);

    IplImage *grey = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    cvCvtColor(img,grey,CV_RGB2GRAY);
    img2mat(tmp[0],grey,0);
    
    genGaussianKernel(gk,5,3.0);
    convolve(tmp[1],tmp[0],gk);
    mat2img(grey,tmp[1],0);
    cvSaveImage("test2.jpg",grey);
    sobelFilter(tmp[2],tmp[3],tmp[1]);
    cart2pol(tmp[0],tmp[1],tmp[2],tmp[3]);
    
    mat2img(grey,tmp[0],0);
    cvSaveImage("test3.jpg",grey);

    
    rep(i,NTMP) cvReleaseMat( &tmp[i] );
    cvReleaseMat( &gk ); 
    cvReleaseImage( &grey ); 
    //cvReleaseImage( &img );
}

int main(int argc, char **argv) {
    try {
        test1();
    } catch (const char *s) {
        printf("Error: %s\n",s);
    }
}

