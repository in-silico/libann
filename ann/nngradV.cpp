#include <octave/oct.h>
#include <cstdio>

DEFUN_DLD(nngradV, args, , "Return the gradient given Y, R, Z homogCoord and V") {
    Matrix Y (args(0).matrix_value());
    Matrix R (args(1).matrix_value());
    Matrix Zt (args(2).matrix_value());
    Matrix V (args(3).matrix_value()); //Matrix V used only for size
    
    int K = Y.cols();
    int H = V.cols()-1;
    int N = Y.rows();
    Matrix dV (K,H+1);
    for (int k=0; k<K; k++)
	for (int h=0; h<(H+1); h++)
	    dV(k,h)=0.0;
    for (int t=0; t<N; t++) {
	for (int i=0; i<K; i++) {
	    for (int h=0; h<(H+1); h++) {
		double tmp = (Y(t,i) - R(t,i)) * Zt(t,h);
		dV(i,h) += tmp;
	    }
	}
    }

    return octave_value(dV);
}


