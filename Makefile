CXX = g++
CXXFLAGS = -Wall -O2 -fopenmp

%.o : %.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

main: main.o nelder_mead.o gp.o quadratic.o rosenbrock.o
	$(CXX) $^ $(CXXFLAGS) -o main

all: main

clean:
	rm -f main *.o

.PHONY : all clean
