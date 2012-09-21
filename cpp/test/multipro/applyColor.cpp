#include <cstdio>
#include <string>
#include <kmeans.h>
#include "global.h"

using namespace std;
using namespace LibAnn;

#define rep(i,n) for(int i=0; i<(n); i++)
#define rep2(i,n) for(int i=0; i<=(n); i++)
#define repf(i,a,b) for(int i=(a); i<=(b); i++)
#define repb(i,a,b) for (int i=(a); i>=b; i--)

int MaxVal = 500;
int MaxCen = 500;

int * applyColor(Mat *ori, Mat *centroids, Mat *dest){
    int *idx = new int[ori->ncols()];
    findClosestCentroids(idx,ori,centroids);
    for(int i=0; i< ori->nrows(); ++i)
        matCopyRow(dest,i,centroids,idx[i]);
    
    return idx;
}

void saveid(int *idx,string name,int Sizeidx){
   FILE *tem = fopen(name.c_str(),"w");
   
   for(int i=0;i < Sizeidx;i++){
        fprintf(tem, "%i ",idx[i]);
   }   
   fclose(tem);
}



int main(int argc , char **argv){
    if(argc != 5){
	    printf("Usage: %s <option> (-i intmat, -c colormat) <orgImg> <centroids> <destImg>\n", argv[0]);
	    return 1;
    }
    
    Mat *ori = new Mat(MaxVal);
    ori->load(argv[2]);
    Mat *centroids = new Mat(MaxCen);
    centroids->load(argv[3]);
    Mat *dest = new Mat(MaxVal);
    
    int *idx = applyColor(ori,centroids,dest);
    if (argv[1][0] == 'c') dest->save(argv[4]);
    else saveid(idx,argv[4],ori->nrows());
    
    return 0;
}
