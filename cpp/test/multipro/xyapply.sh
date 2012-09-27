#! /bin/bash
# parameters are image in and image out

./imgmat.o -M tmp.mat $1
./colorm.o rgb2xy tmp.mat tmp.mat
./applyColor.o -c tmp.mat km.mat tmp.mat
./colorm.o xy2rgb tmp.mat tmp.mat
./imgmat.o -M tmp.mat -I 640 480 $2
rm tmp.mat
