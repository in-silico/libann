#include "extra.h"
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

template<class T>
void initialize_kernel(Image<T> &kernel,int num_kernel = 0){
    for(int i = 0 ; i< 3 ; ++i)
        for(int j = 0; j<3; ++j)
            //kernel(i,j) = (1.0/16.0)*smooth_kernel[i][j];
            kernel(i,j) = laplacian_kernel[i][j];//laplacian_kernel[i][j];//Switch - case.
}


template<class T>
void cp_and_padding(Image<cpx> &ori, Image<T> &img){
    for(int i = 0; i < ori.rows; ++i){
        for(int j = 0; j < ori.cols; ++j){
            if(i < img.rows and j < img.cols)
                ori(i,j) = cpx(img(i,j),0);
            else
                ori(i,j) = cpx(0,0);
        }
    }
}

template<class T>
void multiply(Image<T> &image, Image<T> &kernel, Image<T> &dest){
    if(image.rows !=  kernel.rows or kernel.rows != dest.rows or \
        image.cols !=  kernel.cols or kernel.cols != dest.cols
    )  throw ImgError(3,"Dimentions don't match");
    
    for(int i = 0; i< image.rows; ++i)
        for(int j = 0; j<image.cols; ++j)
            dest(i,j) = image(i,j)*kernel(i,j);
}

template<class T>
void frecuency_multiplication(Image<T> &img,const char *filename, int num_kernel = 0){
    int Ncols = (img.cols == (1<<(log_2(img.cols))-1))? img.cols : 1<<(log_2(img.cols));
    int Nrows = (img.rows == (1<<(log_2(img.rows))-1))? img.rows : 1<<(log_2(img.rows));
    Ncols = Nrows = max(Ncols,Nrows);

    cout<<Ncols<<" "<<Nrows<<endl;
    
    Image<cpx> ori(Nrows,Ncols);
    Image<cpx> kernel(Nrows,Ncols);
    Image<cpx> kernel_frec(Nrows,Ncols);
    Image<T> init_kernel(3,3);
    Image<cpx> dest(Nrows,Ncols);
    Image<cpx> final(Nrows,Ncols);
    Image<T> final2(Nrows,Ncols);

    initialize_kernel(init_kernel,num_kernel);
    cp_and_padding(ori,img);
    cp_and_padding(kernel,init_kernel);
  
    FFT_image(ori,dest);
    FFT_image(kernel,kernel_frec);
    multiply(dest,kernel_frec,final);

    FFT_image(final,ori,-1);
        
    for(int i = 0; i < final.rows; ++i)
        for(int j = 0; j < final.cols; ++j){
            //final2(i,j) = ori(i,j).a / (Nrows*Ncols); // REAL
            final2(i,j) = sqrt(ori(i,j).modsq()) / (Nrows*Ncols); // Magnitude
            assert(ori(i,j).b < 1e-4);
        }
    

    final2.normalize(400);

    save_image(filename, final2);   
}

string explanation = "where options could be : \n -s : smooth kernel, lowpass filter \n -h : highlight kernel, highpass filter \n -sh : sharpen kernel \
\n -p : prewitt kernel \n -so : sobel \n -l : laplacian \n -f : filter with fourier transform";

int main(int argc, char **argv){
    if(argc < 3){
        cout<<"Usage: "<<argv[0]<<" <input> <output> [options]"<<endl<<explanation<<endl;
        return 1;
    }
    Image<float> original = load_image(argv[1]);
    int all = argc==3;    
    for(int i = 3 ; i< argc or all ; ++i){
        Image<float> filtered(original.rows, original.cols);        
        string arg,filename;
        if(argc > 3) arg = argv[i];
        string basename(argv[2]);
        basename = basename.substr(0,basename.size()-4);
        if(arg == "-s" or all){
            filtered = original;
            smooth(filtered);
            filename =  basename + "_smooth.jpg";
            save_image(filename.c_str(), filtered);
        }
        if(arg == "-h" or all){
            filtered = original;
            highlight(filtered,3);
            filename =  basename + "_highlight.jpg";
            save_image(filename.c_str(), filtered);
        }
        if(arg == "-sh" or all){
            filtered = original;
            sharpen(filtered,3);
            filename =  basename + "_sharpen.jpg";
            save_image(filename.c_str(), filtered);
        }
        if(arg == "-p" or all){
            filtered = original;
            prewitt(filtered);
            filename =  basename + "_prewitt.jpg";
            save_image(filename.c_str(), filtered);
        }
        if(arg == "-so" or all){
            filtered = original;
            sobel(filtered);
            filename =  basename + "_sobel.jpg";
            save_image(filename.c_str(), filtered);
        }
        if(arg == "-l" or all){
            filtered = original;
            laplacian(filtered);
            filename =  basename + "_laplacian.jpg";
            save_image(filename.c_str(), filtered);
        }
        if(arg == "-f" or all){
            filename =  basename + "_fourier.jpg";
            frecuency_multiplication(original,filename.c_str());
        }
        all = 0;
    }
    
    return 0;
}
