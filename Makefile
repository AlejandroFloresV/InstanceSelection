all: solver extra

solver:
	mkdir -p bin
	rm -f bin/solver
	c++ src/solver.cpp -o bin/solver

extra:
	mkdir -p bin
	rm -f bin/normalize
	c++ src/normalize.cpp -o bin/normalize
	rm -f bin/parseOutput
	c++ src/parseOutput.cpp -o bin/parseOutput
