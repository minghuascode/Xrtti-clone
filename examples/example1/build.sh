#!/bin/sh

if [ "$1" = "clean" ]; then
    rm -f DumpObjects.o DumpObjectsTest_generated.cpp
    rm -f DumpObjectsTest_generated.o DumpObjectsTest.o DumpObjectsTest
    exit 0
fi

# First, build the DumpObjects object
g++ -g -c DumpObjects.cpp

# Now, generate the DumpObjectsTest Xrtti tables
xrttigen -e '*' -i 'SimpleClass*' -h DumpObjectsTest.h -o DumpObjectsTest_generated.cpp DumpObjectsTest.h

# And, compile it
g++ -g -c DumpObjectsTest_generated.cpp

# Compile the test main
g++ -g -c DumpObjectsTest.cpp

# And link it all together
g++ -o DumpObjectsTest DumpObjectsTest_generated.o DumpObjectsTest.o DumpObjects.o -lxrtti -lessential -lpthread
