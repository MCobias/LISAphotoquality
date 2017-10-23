
INCLUDE_ANALYSER=3rdparty/OpenFace/FaceAnalyser
INCLUDE_LANDMARK=3rdparty/OpenFace/LandmarkDetector

all : compile run

compile :
		g++ -std=c++14 -w -o program `pkg-config opencv --cflags` *.cpp *.hpp utils/*.cpp utils/*.hpp `pkg-config opencv --libs` -ldlib -lblas -lFaceAnalyser -lLandmarkDetector -ltbb -lopenblas -lboost_system -lboost_filesystem -pthread -I$(INCLUDE_ANALYSER) -I$(INCLUDE_LANDMARK)

run :
	./program

clean :
	rm -rf program
