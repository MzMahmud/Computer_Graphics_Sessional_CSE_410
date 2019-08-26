#!/bin/bash

# compileOpenGl.sh
# script to compile in linux

# ----dependencoes----
# sudo apt-get update
# sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev

if [ $# -eq 0 ]
then
	echo "**USAGE**"
	echo "./compileOpenGl.sh file1.cpp file2.cpp ... fileN.cpp"
	exit
fi

CC="g++ -std=c++11"
CFlags="-lglut -lGL -lGLU"

for file in $*
do
    out=`echo "$file" | cut -d"." -f1`
    $CC "$file" $CFlags -o "$out.out"
done

