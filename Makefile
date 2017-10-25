

all : compile run

compile :
		g++ -std=c++14 -w -o program `pkg-config opencv --cflags` *.cpp *.hpp utils/*.cpp utils/*.hpp `pkg-config opencv --libs` -ldlib -lblas -lFaceAnalyser -lLandmarkDetector -ltbb -lboost_filesystem -lopenblas -lboost_system -pthread

run :
	./program

clean :
	rm -rf program
