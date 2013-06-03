#!/bin/sh

if [ $# -gt 0 ] ; then
	base=`basename $1 .cpp`
	echo "compiling $base"
	gcc -ggdb `pkg-config opencv --cflags --libs` $base.cpp -o $base -O2
else
	for i in *.c; do
	    echo "compiling $i"
	    gcc -ggdb `pkg-config --cflags opencv` -o `basename $i .c` $i `pkg-config --libs opencv` -O2;
	done
	for i in *.cpp; do
	    echo "compiling $i"
	    g++ -ggdb `pkg-config --cflags opencv` -o `basename $i .cpp` $i `pkg-config --libs opencv` -O2;
	done
fi
