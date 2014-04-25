all: main

CImage: CImage.hpp
	g++ -o CImage.o CImage.hpp -c

CReader: CImage CReader.hpp
	g++ -o CReader.o CReader.hpp -c

ClassPool: CReader CImage ClassPool.hpp
	g++ -o ClassPool.o ClassPool.hpp -c

Classifier: ClassPool Classifier.hpp
	g++ -o Classifier.o Classifier.hpp -c

MomentClassifier: Classifier MomentClassifier.hpp
	g++ -o MomentClassifier.o MomentClassifier.hpp -c

MomentClassifierId: Classifier MomentClassifierId.hpp
	g++ -o MomentClassifierId.o MomentClassifierId.hpp -c -larmadillo

NNClassifier: Classifier NNClassifier.hpp
	g++ -o NNClassifier.o NNClassifier.hpp -c

main: 	Classifier MomentClassifier MomentClassifierId NNClassifier CReader CImage ClassPool main.cpp
	g++ -o main main.cpp -ggdb -larmadillo

clean:
	rm -rf *.o *~ *# main