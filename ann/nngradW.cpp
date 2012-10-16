#include <octave/oct.h>

DEFUN_DLD(nngradW, args, , "Return the gradient given Y, R, Z, V and Xt") {
    Matrix Y (args(0).matrix_value());
    Matrix R (args(1).matrix_value());
    Matrix Z (args(2).matrix_value());
    Matrix V (args(3).matrix_value()); 
    Matrix Xt (args(4).matrix_value());
    
    int K = Y.cols();
    int H = V.cols()-1;
    int N = Y.rows();
    int D = Xt.cols() - 1;
    Matrix dW (H,D+1);
    for (int h=0; h<H; h++)
	for (int d=0; d<(D+1); d++)
	    dW(h,d)=0.0;
    for (int t=0; t<N; t++) {
	for (int h=0; h<H; h++) {
	    for (int j=0; j<(D+1); j++) {
		double tmp = 0;
		for (int i=0; i<K; i++)
		    tmp += (R(t,i) - Y(t,i))*V(i,h+1);
		double tmp2 = Z(t,h)*(1-Z(t,h))*Xt(t,j);
		dW(h,j) -= tmp*tmp2;
	    }
	}
    }

    return octave_value(dW);
}
