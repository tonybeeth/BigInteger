default: proj

all: proj

clean:
	rm BigInteger.o test.o proj.out

BigInteger.o: BigInteger.h BigInteger.cpp
	g++ -Wall -c BigInteger.cpp

test.o: BigInteger.h test.cpp
	g++ -Wall -c test.cpp

proj: test.o BigInteger.o
	g++ -Wall test.o BigInteger.o -o proj.out
