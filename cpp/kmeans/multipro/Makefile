FLAGS := -O2 -Wall -Wextra
LFLAGS := -I/usr/include/opencv -lml -lcvaux -lhighgui -lcv -lcxcore -lutpann
SRCF := kmeans.cpp applyColor.cpp colorm.cpp imgmat.cpp bright.cpp rgbfeatures.cpp
OBJFILES := $(SRCF:.cpp=.o)
HFILES := $(SRCF:.cpp=.h)
CC := g++

all: $(OBJFILES)

%.o : %.cpp
	$(CC) $(FLAGS) $(LFLAGS) -o $@ $<

clean:
	rm *.o
