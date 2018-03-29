CXX = g++
CXXFLAGS = -Wall -g -fopenmp

%.o : %.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

main: main.o nelder_mead.o gp.o quadratic.o
	$(CXX) $^ $(CXXFLAGS) -o main

all: main

clean:
	rm -f main *.o

.PHONY : all clean
