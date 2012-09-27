#! /bin/bash

./imgmat.o -M tmp.mat $1
./colorm.o rgb2xy tmp.mat tmp.mat
./colorm.o xy2rgb tmp.mat tmp.mat
./imgmat.o -M tmp.mat -I 640 480 $2
rm tmp.mat
