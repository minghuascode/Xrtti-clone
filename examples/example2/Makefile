#!/bin/sh

# src/xrtti-0.4/doc/examples
# quickstart/example2

blddir := $(CURDIR)/../../src/xrtti-0.4/build
cmddir := $(blddir)/bin
incdir := $(blddir)/include
libdir := $(blddir)/lib

srcs := DumpObjectsTest.cpp DumpObjects.cpp 
genh := DumpObjectsTest.h

objs := $(patsubst %.cpp,%.o,$(srcs))

gencpps = $(patsubst %.h,%_gen.cpp,$(genh))
genobjs = $(patsubst %.cpp,%.o,$(gencpps))

allobjs = $(objs) $(genobjs)

ldflags := -L $(libdir) -lxrtti -lpthread
tgt := dmp

.PHONY: clean

# And link it all together
$(tgt): $(allobjs)
	g++ -I $(incdir) -o $@  $(allobjs)  $(ldflags)

# Now, generate the DumpObjectsTest Xrtti tables
%_gen.o: %.h
	LD_LIBRARY_PATH=$(libdir) $(cmddir)/xrttigen \
	    -e '*' -i 'SimpleClass*' -h $< \
	    -o $(patsubst %.o,%.cpp,$@) $<
	- g++ -I $(incdir) -g -c $(patsubst %.o,%.cpp,$@)

# build the DumpObjects object
%.o: %.cpp
	g++ -I $(incdir) -g -c $<

clean:
	rm -f $(allobjs) $(tgt) $(gencpps) gccxml.out

