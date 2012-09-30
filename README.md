Artifitial neural network library.
==================================

So far we have developed the algorithm implementation for K-means clustering, with particular application to images.

The delopment is in three branches:
- singlethread, is a not parallellized development.
- cputhread, is a CPU parallellized development.
- cudabranch, is a GPU parallellized development.

### To Use.

Go to the cpp directory, and use the "make" command for compilation.

	$ cd cpp/
	$ make

You sloud see some like this:

	g++ -O2 -Wall -Wextra -fPIC -c -o matrix.o matrix.cpp
	g++ -O2 -Wall -Wextra -fPIC -c -o kmeans.o kmeans.cpp
	g++ -shared -Wl,-soname,libutpann.so.1 -O2 -Wall -Wextra -fPIC -o libutpann.so matrix.o kmeans.o -lc

in this moment you have a library for K-means algorithm compilated, but is not installed yet, for this use.

	# make install

Now is moment to see the K-means in action, so go to the singlethread branch and in the path cpp/test/colkm for test it with images.
first make sure you have installed the requirements to process the images. You can install them with the following line.

	# aptitude install libcvaux-dev libhighgui-dev libcv-dev
	
_______
Developed by insilico.
