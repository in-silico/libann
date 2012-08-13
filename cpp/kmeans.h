#ifndef KMEANS_H
#define KMEANS_H

#include "matrix.h"

namespace LibAnn {

    /**
     * Runs the k-means algorithm over the given data matrix x (Each column is a instance)
     * and returns a k column vector with the chosen centers. It iterates until each data instance does
     * not change of center or for maxIter iterations.
     */
    int kmeans(Mat *centers, const Mat *x, const Mat *initial_centroids, int maxIter);

    /**
     * Creates a k-dimensional vector centroids picking randomly from x a data point for each centroid and
     * making sure that no data point is chosen twice. 
     */
    void kmeansInit(Mat *centroids, const Mat *x, int k);

    /**
     * Normalices each column of org to have zero mean and unit standard deviation, and stores the
     * result to dest.
     */
    void normalize(Mat *dest, const Mat *org);

    /**
     * Finds the closest centroids to each instance of x in idx. Meaning that idx[j] is a number
     * between 0 and k-1 indicating which centroid (row vector) is closer to x[j]. The vector idx
     * is a integer vector of size greater or equal to the number of rows of X.
     */
    void findClosestCentroids(int *idx, const Mat *x, const Mat *centroids);

    /**
     * Computes each centroid j as the mean of the data instances whose idx[i]==j.
     */
    void computeCentroids(Mat *centroids, const Mat* x, int *idx, int k);


};
#endif
