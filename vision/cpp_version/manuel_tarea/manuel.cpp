#include "extra.h"
#include "img_lib.h"
#include <cmath>
#include <string>
#include <iostream>

template<class T>
void save_image(const char *s, Image<T> &img) {
    int m = img.cols, n = img.rows;
    IplImage *cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    img2cvimg(cvg, img);
    cvSaveImage(s,cvg);
}

Image<float> load_image(char *filename){
    IplImage *cvi = cvLoadImage(filename);
    int m = cvi->width, n = cvi->height;
    Image<float> img(n,m);
    cvimg2img(img, cvi);
    return img;
}

void FFT_test() {
    IplImage *cvi = cvLoadImage("test.jpg");
    int m = cvi->width, n = cvi->height;
    IplImage *cvg = cvCreateImage(cvSize(m,n),IPL_DEPTH_8U,1);
    cvCvtColor(cvi,cvg,CV_RGB2GRAY);
    Image<float> img(n,m);
    Image<float> img2(n,m);
    cvimg2img(img, cvg);
    int Ncols = (img.cols == (1<<(log_2(img.cols))-1))? img.cols : 1<<(log_2(img.cols));
    int Nrows = (img.rows == (1<<(log_2(img.rows))-1))? img.rows : 1<<(log_2(img.rows));
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

    int tmp=5;
    for (int i=-tmp; i<=tmp; i++) for (int j=-tmp; j<=tmp; j++) {
        dest((i+Nrows)%Nrows, (j+Ncols) % Ncols) = 0;
    }
    FFT_image(dest,ori,-1);
    
    dest = ori;
    for(int i = 0; i < img2.rows; ++i)
        for(int j = 0; j < img2.cols; ++j)
            img2(i,j) = sqrt(dest(i,j).modsq()) / (Nrows*Ncols);
            
    //img2.normalize(255);

    save_image("salida.jpg", img2);
}


int smooth_kernel[3][3] = {{1 , 2, 1} , {2, 4, 2} , {1, 2, 1} };

template<class T>
void smooth(Image<T> &org){
    Image<T> kernel(3,3);
    for(int i = 0 ; i< 3 ; ++i)
        for(int j = 0; j<3; ++j)
            kernel(i,j) = (1.0/16.0)*smooth_kernel[i][j];
    org.convolve(kernel);
}

int highlight_kernel1[3][3] = {{1 , -2, 1} , {-2, 4, -2} , {1, -2, 1} };
int highlight_kernel2[3][3] = {{-2 , 0, -2} , {0, 8, 0} , {-2, 0, -2} };
int highlight_kernel3[3][3] = {{0 , -1, 0} , {-1, 4, -1} , {0, -1, 0} };

template<class T>
void highlight(Image<T> &org, int numker = 1){
    Image<T> kernel(3,3);
    for(int i = 0 ; i< 3 ; ++i)
        for(int j = 0; j<3; ++j)
            kernel(i,j) = (numker!=1)?(numker==2)? highlight_kernel2[i][j]:highlight_kernel3[i][j] : highlight_kernel1[i][j];
    org.convolve(kernel);
}


int sharpen_kernel1[3][3] = {{1 , -2, 1} , {-2, 5, -2} , {1, -2, 1} };
int sharpen_kernel2[3][3] = {{-1 , -1, -1} , {-1, 9, -1} , {-1, -1, -1} };
int sharpen_kernel3[3][3] = {{0 , -1, 0} , {-1, 5, -1} , {0, -1, 0} };

template<class T>
void sharpen(Image<T> &org, int numker = 1){
    Image<T> kernel(3,3);
    for(int i = 0 ; i< 3 ; ++i)
        for(int j = 0; j<3; ++j)
            kernel(i,j) = (numker!=1)?(numker==2)? sharpen_kernel2[i][j]:sharpen_kernel3[i][j] : sharpen_kernel1[i][j];
    org.convolve(kernel);
}

int prewitt_dx[3][3] = {{-1 , -1, -1} , {0, 0, 0} , {1, 1, 1}};
int prewitt_dy[3][3] = {{-1 , 0, 1} ,{-1 , 0, 1} , {-1 , 0, 1}};

template<class T>
void prewitt(Image<T> &org){
    Image<T> kernel(3,3);
    for(int k=0; k<2; ++k){
        for(int i = 0 ; i< 3 ; ++i)
            for(int j = 0; j<3; ++j)
                kernel(i,j) = (k&0) ? prewitt_dx[i][j] : prewitt_dy[i][j];
        org.convolve(kernel);
    }
}

int sobel_dx[3][3] = {{-1 , -2, -1} , {0, 0, 0} , {1, 2, 1}};
int sobel_dy[3][3] = {{-1 , 0, 1} ,{-2 , 0, 2} , {-1 , 0, 1}};

template<class T>
void sobel(Image<T> &org){
    Image<T> kernel(3,3);
    for(int k=0; k<2; ++k){
        for(int i = 0 ; i< 3 ; ++i)
            for(int j = 0; j<3; ++j)
                kernel(i,j) = (k&0) ? sobel_dx[i][j] : sobel_dy[i][j];
        org.convolve(kernel);
    }
}

int laplacian_kernel[3][3] = {{-1 , -2, -1} , {0, 0, 0} , {1, 2, 1}};

template<class T>
void laplacian(Image<T> &org){
    Image<T> kernel(3,3);
    for(int i = 0 ; i< 3 ; ++i)
        for(int j = 0; j<3; ++j)
            kernel(i,j) = laplacian_kernel[i][j];
            
    org.convolve(kernel);
    
}

int main(int argc, char **argv){
    if(argc != 3){
        cout<<"Usage: "<<argv[0]<<" <input> <output>"<<endl;
        return 1;
    }
    Image<float> original = load_image(argv[1]);
    //Image<float> smoothed = smooth(original);
    /**
    * Multiple image definitions. 
    */
    Image<float> smoothed(original.rows, original.cols);
    Image<float> high1(original.rows, original.cols);
    Image<float> high2(original.rows, original.cols);
    Image<float> high3(original.rows, original.cols);
    Image<float> sharpen1(original.rows, original.cols);
    Image<float> sharpen2(original.rows, original.cols);
    Image<float> sharpen3(original.rows, original.cols);
    Image<float> prewitt1(original.rows, original.cols);
    Image<float> sobel1(original.rows, original.cols);
    Image<float> laplacian1(original.rows, original.cols);
    
    smoothed = original;
    high1 = original;
    high2 = original;
    high3 = original;
    sharpen1 = original;
    sharpen2 = original;
    sharpen3 = original;
    prewitt1 = original;
    sobel1 = original;
    laplacian1 = original;
    
    smooth(smoothed);
    highlight(high1);
    highlight(high2,2);
    highlight(high3,3);
    sharpen(sharpen1);
    sharpen(sharpen2,2);
    sharpen(sharpen3,3);
    prewitt(prewitt1);
    sobel(sobel1);
    laplacian(laplacian1);
    
    //smoothed.normalize();
    save_image("original.jpg", original);
    save_image("smoothed.jpg", smoothed);
    save_image("high1.jpg", high1);
    save_image("high2.jpg", high2);
    save_image("high3.jpg", high3);
    save_image("sharpen1.jpg", sharpen1);
    save_image("sharpen2.jpg", sharpen2);
    save_image("sharpen3.jpg", sharpen3);
    save_image("prewitt.jpg", prewitt1);
    save_image("sobel.jpg", sobel1);
    save_image("laplacian.jpg", laplacian1);
    
//    FFT_test();
    
    return 0;
}
