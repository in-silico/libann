#include "logistic_regression.h"


namespace LAnn{
    
    
    void sigmoid(Mat *z, Mat *g){
        for(int i=0 ; i<z.nrows() ; i++){
            rep( j=0 ; j<z.ncols() ; j++){
                g.get(i,j) = 1/(1 + exp(-1 * z.get(i,j)));
            }
        }

    }

    void sigmoidGradient(Mat *z,Mat *g){
        double tmp=0;
        sigmoid(z, g);
        
        rep (i, g.nrows()){
            rep (j, g.ncols()){
                double tmp= g.get(i,j);
                g.get(i,j)= tmp*(1-tmp);
                }
           }
                
    }

    void regerrnn(NeuronalN nn, Mat *x, Mat *R, Mat *E){


    }

    void classerrnn(NeuronalN nn, Mat *x, Mat *R, Mat *E){


    }

    void regnn(NeuronalN nn, Mat *x, Mat *y, Mat *z){

    }

    void nntrain(NeuronalN nn, Mat *x, Mat *r, int iter, void (* compnn )(NeuronalN nn, Mat *x, Mat *y, Mat *z), \
                     void (* errnn)(NeuronalN nn,Mat *x, Mat *R, Mat *E)){
                     
                     
    }

    void nngradV(Mat *y, Mat *r,Mat *Zt, Mat *v, Mat *dv){

    }

    void nngradW(Mat *y, Mat *r,Mat *Z, Mat *v,Mat *xt, Mat *dw){


    }

    void nngrad(NeuronalN nn,Mat *wv, Mat *x, Mat *r, void (* compnn )(NeuronalN nn, Mat *x, Mat *y, Mat *z), \
                     void (* errnn)(NeuronalN nn,Mat *x, Mat *R, Mat *E),Mat * E,Mat *grad){
                     
                     
                     
    }
}
