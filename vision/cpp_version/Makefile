
INC := /usr/include/opencv
LIB := -lcv -lhighgui
FLAGS := -g

all: test extra ransac

test: img_test.cpp img_lib.h homography.h calibration.h 
	g++ img_test.cpp -o test -I$(INC) $(LIB) $(FLAGS)

ransac: test_ransac.cpp ransac.h
	g++ test_ransac.cpp -o ransac -I$(INC) $(LIB) $(FLAGS)

extra: extra_test.cpp extra.h
	g++ extra_test.cpp -o extra -I$(INC) $(LIB) $(FLAGS)
