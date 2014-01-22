Instance Selection Solver
=========================

##Compile

Just run `make` in the root folder. This command will compile everything you need to run the different solutions for the Instance Selection problem.

## Running

To run a particular metaheuristic against some of the datasets provided just type:

	src/solver <algorithm> <dataset> <# 10-fcv set>

Where:

- **Algorithm**: `GGA`, `SGA`, `CHC`, `StandardPBIL`, `RandomPBIL`, `ClosestEnemyPBIL` or `PSO`.
- **Dataset**: `iris`, `wdbc` or `segmentation`.
- **10-fcv set**: a number between [0,9].

You can also run a *Full Factorial Experiment* for every 10-fcv set by typing:

	./run.sh <algorithm> <dataset>
