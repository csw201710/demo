#!/bin/bash
if [ ! -d 'build' ];
then
	mkdir build || exit 1
fi
cd build || exit 1
cmake .. && make VERBOSE=1 $*
