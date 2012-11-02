#include "logistic_regression.h"
#include <ctime>


namespace LAnn{

    
    NeuronalN::NeuronalN(int d,int h, int k){
        srand48(time(0));
        
        w = new Mat(h*(d+1));
        v = new Mat(k*(h+1));
        
        w->setSize(h,d+1);
        v->setSize(k,h+1);
        
        for(int i=0; i < h; ++i){
            for(int j=0; j < (d+1); ++j){
                w->get(i,j)  = (drand48() - 0.5)*(2.0/100.0);
            }
        }
        
        for(int i=0; i < k; ++i){
            for(int j=0; j < (h+1); ++j){
                v->get(i,j)  = (drand48() - 0.5)*(2.0/100.0);
            }
        }
    }
    
    void sigmoid(Mat *z, Mat *g){


    }

    void sigmoidGradient(Mat *z,Mat *g){


    }

    void regerrnn(NeuronalN nn, Mat *x, Mat *R, Mat *E){


    }

    void classerrnn(NeuronalN nn, Mat *x, Mat *R, Mat *E){


    }

    void regnn(NeuronalN nn, Mat *x, Mat *y, Mat *z){

    }

    void nntrain(NeuronalN nn, Mat *x, Mat *r, int iter, void (* compnn)(NeuronalN nn, Mat *x, Mat *y, Mat *z), \
                     void (* errnn)(NeuronalN nn,Mat *x, Mat *R, Mat *E)){
                     
                     
    }

    void nngradV(Mat *y, Mat *r,Mat *Zt, Mat *v, Mat *dv){

    }

    void nngradW(Mat *y, Mat *r,Mat *Z, Mat *v,Mat *xt, Mat *dw){


    }

    void nngrad(NeuronalN nn,Mat *wv, Mat *x, Mat *r, void (* compnn)(NeuronalN nn, Mat *x, Mat *y, Mat *z), \
                     void (* errnn)(NeuronalN nn,Mat *x, Mat *R, Mat *E),Mat * E,Mat *grad){
                     
                     
                     
    }
}

