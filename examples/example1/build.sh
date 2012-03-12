#!/bin/sh

# src/xrtti-0.4/doc/examples
# quickstart/example1

blddir=$PWD/../../src/xrtti-0.4/build
cmddir=$blddir/bin
incdir=$blddir/include
libdir=$blddir/lib


if [ "$1" = "clean" ]; then
    rm -f DumpObjects.o DumpObjectsTest_generated.cpp
    rm -f DumpObjectsTest_generated.o DumpObjectsTest.o DumpObjectsTest
    exit 0
fi

# First, build the DumpObjects object
g++ -I $incdir -g -c DumpObjects.cpp
if [ ! "x$?" == "x0" ]; then exit 1; fi

# Now, generate the DumpObjectsTest Xrtti tables
LD_LIBRARY_PATH=$libdir $cmddir/xrttigen -e '*' -i 'SimpleClass*' -h DumpObjectsTest.h -o DumpObjectsTest_generated.cpp DumpObjectsTest.h
if [ ! "x$?" == "x0" ]; then exit 2; fi

# And, compile it
g++ -I $incdir -g -c DumpObjectsTest_generated.cpp
if [ ! "x$?" == "x0" ]; then exit 3; fi

# Compile the test main
g++ -I $incdir -g -c DumpObjectsTest.cpp
if [ ! "x$?" == "x0" ]; then exit 4; fi

# And link it all together
g++ -I $incdir -o DumpObjectsTest DumpObjectsTest_generated.o DumpObjectsTest.o DumpObjects.o -L $libdir -lxrtti -lpthread
if [ ! "x$?" == "x0" ]; then exit 5; fi
