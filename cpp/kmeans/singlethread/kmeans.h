#ifndef KMEANS_H
#define KMEANS_H

#include "matrix.h"

namespace LibAnn {

    struct KMeansConf {
	int maxIter;
	int *pidx;
	int nthreads;

	KMeansConf() {
	    maxIter=100;
	    pidx=0;
	    nthreads=1;
	}
    };

    /**
     * Runs the k-means algorithm over the given data matrix x (Each column is a instance)
     * and returns a k column vector with the chosen centers. It iterates until each data instance does
     * not change of center or for maxIter iterations.
     */
    int kmeans(Mat *centers, Mat *x, Mat *initial_centroids, KMeansConf *conf=0);
     
    /**
     * Compute the mean squared error of projecting x on centers
     */
    double kmeansError(Mat *centers, Mat *x);

    /**
     * Creates a k-dimensional vector centroids picking randomly from x a data point for each centroid and
     * making sure that no data point is chosen twice.
     */
    void kmeansInit(Mat *centroids, Mat *x, int k);

    /**
     * Creates a random permutation of length k out of n elements
     */
    void randPerm(int *ans, int k, int n);

    /**
     * Computes the standar deviation and mean vector of each column of x into ans
     */
    void stdv(Mat *stdev, Mat *mean, Mat *x);

    /**
     * Normalices each column of org to have zero mean and unit standard deviation, and stores the
     * result to dest. The mean and vector of standard deviations is returned as a row vector.
     */
    void normalize(Mat *dest, Mat *mean, Mat *stdev, Mat *org);
    
    /**
     * Finds the closest centroids to each instance of x in idx. Meaning that idx[j] is a number
     * between 0 and k-1 indicating which centroid (row vector) is closer to x[j]. The vector idx
     * is a integer vector of size greater or equal to the number of rows of X.
     */
    bool findClosestCentroids(int *idx, Mat *x, Mat *centroids);

    /**
     * Computes each centroid j as the mean of the data instances whose idx[i]==j.
     */
    void computeCentroids(Mat *centroids, Mat* x, const int *idx, int k);


};
#endif
