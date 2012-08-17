VERSION=1
RELEASE=0.1
DIR=/usr/local/lib
HDIR=/usr/local/include
FLAGS=-O2 -Wall -Wextra -fPIC
SRCF := matrix.cpp kmeans.cpp
OBJFILES := $(SRCF:.cpp=.o)
HFILES := $(SRCF:.cpp=.h)
CC := g++
LIB := libutpann.so

$(LIB) : $(OBJFILES)
	$(CC) -shared -Wl,-soname,$(LIB).$(VERSION) $(FLAGS) -o $(LIB) $(OBJFILES) -lc

%.o : %.cpp
	g++ $(FLAGS) -c -o $@ $<

install : $(LIB) $(HFILES)
	cp $(LIB) $(DIR)/$(LIB).$(VERSION)
	ln -s $(DIR)/$(LIB).$(VERSION) $(DIR)/$(LIB)
	ln -s $(DIR)/$(LIB).$(VERSION) $(DIR)/$(LIB).$(VERSION).$(RELEASE)
	cp $(HFILES) $(HDIR)
	ldconfig

uninstall:
	rm $(DIR)/$(LIB)*
	for i in $(HFILES); do rm $(HDIR)/$(HFILES); done
	ldconfig

clean:
	rm -f $(LIB) $(OBJFILES)
