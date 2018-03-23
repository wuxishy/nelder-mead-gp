CXX = g++
CXXFLAGS = -Wall

%.o : %.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

main: main.o nelder_mead.o
	$(CXX) $^ $(CXXFLAGS) -o main

all: main

clean:
	rm -f main *.o

.PHONY : main
