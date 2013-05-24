
#include <cv.h>
#include <highgui.h>

template<class T>
class Image {
    T *data;
    int rows,cols;
public:
    Image(int row, int cols);
    ~Image();
    T &operator()(int i, int j);
    T *getRow(int r);
    void convolve(Image &kernel);
    void operator=(Image &other);
    void normalize(int minVal=0, int maxVal=255);
};

template<class T>
void gaussianFilter(Image<T> *dest, Image<T> *org, double sigma);

template<class T>
void sobelFilter(Image<T> *dx, Image<T> *dy, Image<T> *org);

template<class T>
void harrisFilter(Image<T> *dest, Image<T> *org, int w, double k=0.01);

template<class T>
void cart2pol(Image<T> *r, Image<T> *theta, Image<T> *x, Image<T> *y);

template<class T>
void img2mat(IplImage *ocv, Image<T> *img);

template<class T>
void mat2img(Image<T> *img, IplImage *ocv);
