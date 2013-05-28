#include "extra.h"
#include "img_lib.h"
#include <cmath>

template<class T>
void saveImg(const char *s, Image<T> &img) {
    int m = img.cols, n = img.rows;
    IplImage *cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    img2cvimg(cvg, img);
    cvSaveImage(s,cvg);
}

void test1() {
    IplImage *cvi = cvLoadImage("test.jpg");
    int m = cvi->width, n = cvi->height;
    IplImage *cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    cvCvtColor(cvi,cvg,CV_RGB2GRAY);
    Image<float> img(n,m);
    Image<float> img2(n,m);
    cvimg2img(img, cvg);
    int Ncols = (img.cols == (1<<(log_2(img.cols))-1))? img.cols : 1<<(log_2(img.cols));
    int Nrows = (img.rows == (1<<(log_2(img.rows))-1))? img.rows : 1<<(log_2(img.rows));
    printf("Cols: %d %d\n", img.cols,Ncols);
    printf("Rows: %d %d\n", img.rows,Nrows);
    Image<cpx> ori(Nrows,Ncols);
    Image<cpx> dest(Nrows,Ncols);
    
    for(int i = 0; i < Nrows; ++i){
        for(int j = 0; j < Ncols; ++j){
            if(i < img.rows and j < img.cols)
                ori(i,j) = cpx(img(i,j),0);
            else
                ori(i,j) = cpx(0,0);
        }
    }
    
    FFT_image(ori,dest);
    int tmp=30;
    for (int i=-tmp; i<=tmp; i++) for (int j=-tmp; j<=tmp; j++) {
        dest((i+Nrows)%Nrows, (j+Ncols) % Ncols) = 0;
    }
    FFT_image(dest,ori,-1);
    
    dest = ori;
    for(int i = 0; i < img2.rows; ++i)
        for(int j = 0; j < img2.cols; ++j)
            img2(i,j) = sqrt(dest(i,j).modsq()) / (Nrows*Ncols);
            
    //img2.normalize(255);

    saveImg("test2.jpg", img2);
}

int main() {
    test1();
    return 0;
}
