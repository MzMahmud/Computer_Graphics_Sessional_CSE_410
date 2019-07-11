CC=g++ -std=c++11

all: main.o matrix.o point.o vector.o
	$(CC) main.o matrix.o point.o vector.o -o main.out

main.o: main.cpp
	$(CC) -c main.cpp -o main.o

matrix.o: matrix.cpp matrix.h
	$(CC) -c matrix.cpp -o matrix.o

point.o: point.cpp point.h
	$(CC) -c point.cpp -o point.o

vector.o: vector.cpp vector.h
	$(CC) -c vector.cpp -o vector.o

compare: compare_file.cpp
	$(CC) compare_file.cpp -o compare.out

clean:
	rm -rf *.o *.out
