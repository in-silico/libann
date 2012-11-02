#include "logistic_regression.h"


namespace LAnn{
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

    void nntrain(NeuronalN nn, Mat *x, Mat *r, int iter, void (compnn *)(NeuronalN nn, Mat *x, Mat *y, Mat *z), \
                     void (* errnn)(NeuronalN nn,Mat *x, Mat *R, Mat *E)){
                     
                     
    }

    void nngradV(Mat *y, Mat *r,Mat *Zt, Mat *v, Mat *dv){

    }

    void nngradW(Mat *y, Mat *r,Mat *Z, Mat *v,Mat *xt, Mat *dw){


    }

    void nngrad(NeuronalN nn,Mat *wv, Mat *x, Mat *r, void (compnn *)(NeuronalN nn, Mat *x, Mat *y, Mat *z), \
                     void (* errnn)(NeuronalN nn,Mat *x, Mat *R, Mat *E),Mat * E,Mat *grad){
                     
                     
                     
    }
}
