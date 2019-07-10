CC=g++ -std=c++11

all: main.o matrix.o point.o
	$(CC) main.o matrix.o point.o -o main.out

main.o: main.cpp
	$(CC) -c main.cpp -o main.o

matrix.o: matrix.cpp matrix.h
	$(CC) -c matrix.cpp -o matrix.o

point.o: point.cpp point.h
	$(CC) -c point.cpp -o point.o

clean:
	rm -rf *.o *.out
