
#ifndef RANSAC_H
#define RANSAC_H

#include <cmath>
#include <cstdlib>
#include <cv.h>

class Ransac {
public:
    int n; //number of points to take to estimate the model
    int m; //minimum acceptable for the number of inliers
    double rho; //tolerance to determine if a instance is outlier or not
    double p; //desired probability that all selected points are inliers
    int ndata; //total number of data points available to ransac

    /**
     * Computes the model given a array of integers between 0 and ndata-1 representing
     * the indexes of the data point values. If there was a previously computed model it
     * is safe to throw it.
     */
    virtual void computeModel(int *indexes, int nindexes) = 0;

    /**
     * Computes the error of the instance indexed by ix from the currently computed model
     */
    virtual double compError(int ix) = 0;

    /**
     * Ransac will call this method when the currently found model is better than those found
     * previously. This method should then save the current model in a "best model" variable so
     * it can not be overwritten by computeModel()
     */
    virtual void saveCurrentModel() = 0;

    // Set current model equal to the best saved model
    virtual void useSavedModel() = 0;

    Ransac(int n, int m, int ndata, double rho);
   
    //return in indexes a reandom set of m elements
    void randSet(int *indexes);

    //returns the size of the support set for the current model
    int supportSize(int *indexes=0);

    //runs ransac to fit a model robust to outliers
    void ransac();
};

//implementation

Ransac::Ransac(int n, int m, int ndata, double rho) {
    if (ndata<=n) throw "Too few data point for the given subset size n";
    this->n=n; this->m=m; this->ndata=ndata; this->rho=rho;
    p = 0.99; //set to 99%, but can be changed later if required
    srand( time(0) );
}

void Ransac::randSet(int *indexes) {
    int i=0;
    while (i<n) {
        int r = rand() % ndata;
        char ok=1;
        for (int j=0; j<i; j++) if (r==indexes[j]) ok=0;
        if (ok) {
            indexes[i++] = r;
        }
    }
}

int Ransac::supportSize(int *indexes) {
    int ans=0;
    for (int i=0; i<ndata; i++) {
        double tmp = compError(i);
        if (tmp < rho) {
            if (indexes != 0) indexes[ans]=i;
            ans++;
        }
    }
    return ans;
}

void Ransac::ransac() {
    double N = 1e100;
    int iter=0;
    int* subset = new int[ndata];
    int bestSupp = 0;
    while ( (iter < N) && (bestSupp < m) ) {
        randSet(subset);
        computeModel(subset,n);
        int supp = supportSize();
        if (supp > bestSupp) {
            saveCurrentModel();
            bestSupp=supp;
        }
        iter++;
        double w = bestSupp/((double)ndata); //current probability that a datapoint is inlier
        N = log(1-p)/log(1-pow(w,n)); //update to current maximum number of iterations
    }
    //after finishing, recompute the model with all the inliers
    useSavedModel();
    int inliers = supportSize(subset);
    computeModel(subset,inliers);
    saveCurrentModel();
    delete [] subset;
}

#endif
