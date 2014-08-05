Instance Selection Solver
=========================

##Compile

Just run `make` in the root folder. This command will compile everything you need to run the different solutions for the Instance Selection problem.

## Running

To run a particular metaheuristic against some of the datasets provided, just type:

	bin/solver [-<cmd> <value>]

Where:

	-alg    Algorithm to run. Options: GGA, SGA, CHC, PBIL, PSO,
			CNN, ClosestNE, FarthestNE, and NEHS.
	-f      File from where the data will be loaded.
	-10fcv  Index for 10-fcv [0,9]. If not indicated, it uses
			the entire dataset for training.
	-iter   Maximum number of iterations (default 10000).
	-seed   An unsigned integer value to be used as seed by the
			pseudo-random number generator (default time(NULL)).
	-pop    Size of the population (default 50).
	-alpha  Importance of error over reducion for the calculation
			of the fitness function (default 0.5).
	-bit    Probability of appearance for each bit (default 0.05).
	-init   Policy of inicialization for the population, options:
			Random (default), ClosestNE, FarthestNE, CNN and NEHS.
	-mp     Mutation probability (default 0.001).
	-ms     Mutation shift (Only PBIL) (default 0.01).
	-lr     Learning Rate (Only PBIL) (default 0.2).
	-nlr    Negative Learning Rate (Only PBIL) (default 0.075).
	-pso    PSO particles (default 15).
	-vmax   Maximum value for PSO velocity (Only PSO) (default 0.05).
	-c1     Weight for local best  (Only PSO) (default 0.2).
	-c2     Weight for global best (Only PSO) (default 0.2).
	-w      Inertia weight (Only PSO) (default 0.6).
	-cross  Crossover probability (default 1.0).
	-ex     % of instances excluded from NEHS (default 0.33333).

You can also run a *Full Factorial Experiment* for every 10-fcv set by typing:

	./full.sh [-<cmd> <value>]
