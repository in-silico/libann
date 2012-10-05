#! /bin/bash

find pic/f* -name "*.jpg" | xargs ./imgmat.o -M tmp.mat -S 200000
./colorm.o rgb2xy tmp.mat tmp.mat
./kmeans.o -K 16 -I 100000 -T 10 -N 2 tmp.mat
rm tmp.mat

