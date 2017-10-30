

all : compile run

compile :
		g++ -std=c++14 -w -o program `pkg-config opencv --cflags`\
		 *.cpp *.hpp utils/*.cpp utils/*.hpp `pkg-config opencv --libs` -ldlib -pthread

run :
	./program

clean :
	rm -rf program
