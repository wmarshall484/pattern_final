all: main

CImage: CImage.hpp
	g++ -o CImage.o CImage.hpp -c

CReader: CImage CReader.hpp
	g++ -o CReader.o CReader.hpp -c

ClassPool: CReader CImage ClassPool.hpp
	g++ -o ClassPool.o ClassPool.hpp -c

main: CReader CImage ClassPool main.cpp
	g++ -o main main.cpp

clean:
	rm -rf *.o *~ *# main