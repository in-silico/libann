#ifndef LOG_REG_H
#define LOG_REG_H

#include "../matrix.cpp"

/**
* Nota de programación:
* Para las funciones pimero escribir los parámetros de entrada
* y luego los de salida, ejemplo
* void fun(Mat *entrada,Mat *salida);
* Borrar esto.
*/

namespace LAnn{
    
    /**
    * Class for Neuronal Network Representation
    */
    class NeuronalN{

        public:
            Mat *w;
            Mat *v;
            
            /**
            * Creates a neuronal network with D and H neurons on the input and hidden
            * layer respectively and K outputs.
            */
            NeuronalN(int d,int h, int k){}
    };
    
    /**
    * Computes de sigmoid for given z
    */
    void sigmoid(Mat *z, Mat *g);
    
    
    /**
    * returns the gradient of the sigmoid function
    * evaluated at z.
    * g = SIGMOIDGRADIENT(z) computes the gradient of the sigmoid function
    * evaluated at z. This should work regardless if z is a matrix or a
    * vector. In particular, if z is a vector or matrix, you should return
    * the gradient for each element.
    */
    void sigmoidGradient(Mat *z,Mat *g);
    
    
    /**
    * Computes the error function for regression, returns in the E matrix.
    */
    void regerrnn(NeuronalN nn, Mat *x, Mat *R, Mat *E);
    
    /**
    * Computes the y values for given Z in the Neuronal Network.
    */
    void regnn(NeuronalN nn, Mat *x, Mat *y, Mat *z);
    
    /**
    * Trains the NeuronalNetwork with determinated X and R, compnn and errnn are generic functions.
    */
    
    void nntrain(NeuronalN nn, Mat *x, Mat *r, int iter, void (compnn *)(NeuronalN nn, Mat *x, Mat *y, Mat *z), \
                 void (* errnn)(NeuronalN nn,Mat *x, Mat *R, Mat *E));
                
    /**
    * Return the gradient given Y, R, Z homogCoord and V
    * Matrix V used only for size
    */
                 
    void nngradV(Mat *y, Mat *r,Mat *Zt, Mat *v, Mat *dv);
    
    /**
    * Return the gradient given Y, R, Z, V and Xt
    */
    
    void nngradW(Mat *y, Mat *r,Mat *Z, Mat *v,Mat *xt, Mat *dw);
    
    /**
    * Implements the neural network cost function for a two layer
    * neural network which performs classification.
    */
    void nngrad(NeuronalN nn,Mat *wv, Mat *x, Mat *r, void (compnn *)(NeuronalN nn, Mat *x, Mat *y, Mat *z), \
                 void (* errnn)(NeuronalN nn,Mat *x, Mat *R, Mat *E),Mat * E,Mat *grad);
    
};


#endif
