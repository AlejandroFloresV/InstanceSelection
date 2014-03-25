Instance Selection Solver
=========================

##Compile

Just run `make` in the root folder. This command will compile everything you need to run the different solutions for the Instance Selection problem.

## Running

To run a particular metaheuristic against some of the datasets provided, just type:

	bin/solver [-<cmd> <value>]

Where:

	-alg   Algorithm to run. Options: GGA, SGA, CHC,
		   PSO, PBIL, PBILwithHUX and FarEnemyVoronoi.
	-f     File from where the data will be loaded.
	-10fcv Index for 10-fcv [0,9]. If not indicated, it uses
		   the entire dataset for training.
	-iter  Maximum number of iterations (default 10000).
	-seed  An unsigned integer value to be used as seed by the
		   pseudo-random number generator (default time(NULL)).
	-pop   Size of the population (default 50).
	-init  Policy of inicialization for the population, options:
		   Random (default), ClosestEnemy, FarthestEnemy,
		   and FarEnemyVoronoi.
	-mp    Mutation probability (default 0.001).
	-ms    Mutation shift (Only PBIL) (default 0.01).
	-lr    Learning Rate (Only PBIL) (default 0.2).
	-nlr   Negative Learning Rate (Only PBIL) (default 0.075).
	-pso   PSO particles (default 15).
	-c1    Weight for local best (Only PSO) (default 3.0).
	-c2    Weight for global best (Only PSO) (default 1.0).
	-cross Crossover probability (default 1.0).
	-ex    % of Closest Enemy instances for the FarEnemyVoronoi
		   algorithm (default 0.33333).

You can also run a *Full Factorial Experiment* for every 10-fcv set by typing:

	./full.sh [-<cmd> <value>]
