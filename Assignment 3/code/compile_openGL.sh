#!/bin/bash

if [ $# -eq 0 ]
then
	echo "**USAGE**"
	echo "./compileOpenGl.sh file1.cpp file2.cpp ... fileN.cpp"
	exit
fi

CC="g++ -std=c++11"
CFlags="-lglut -lGL -lGLEW -lGLU"

for file in $*
do
    out=`echo "$file" | cut -d"." -f1`
    $CC "$file" $CFlags -o "$out.out"
done
