
INC := /usr/include/opencv
LIB := -lcv -lhighgui
FLAGS := -g

all: test extra

test: img_test.cpp img_lib.h 
	g++ img_test.cpp -o test -I$(INC) $(LIB) $(FLAGS)

extra: extra_test.cpp extra.h
	g++ extra_test.cpp -o extra -I$(INC) $(LIB) $(FLAGS)
