
INC := /usr/include/opencv
LIB := -lcv -lhighgui
FLAGS := -O2

all: test

test: test.cpp filter.cpp
	g++ test.cpp -o test -I$(INC) $(LIB) $(FLAGS)

perspective: perspective.cpp
	g++ perspective.cpp -o perspective -I$(INC) $(LIB) $(FLAGS)

