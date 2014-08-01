#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <limits>
#include <cstring>
#include <algorithm>
#include <vector>
#include <set>
#include <thread>
#include "conf.h"
#include "tenfold.h"
#include "chromosome.h"
#include "dist.h"
#include "kd-tree.h"
#include "onenn.h"
#include "is-alg.h"
#include "init.h"
#include "genetic.h"
#include "pbil.h"
#include "pso.h"

using namespace std;

string algorithm, filepath;
int tenfcv = -1;

#define ifi(str,var) if (cmd==str) { i++; if (i==argc) FatalError(eP); var=atoi(argv[i]); }
#define iff(str,var) if (cmd==str) { i++; if (i==argc) FatalError(eP); var=atof(argv[i]); }
#define ifs(str,var) if (cmd==str) { i++; if (i==argc) FatalError(eP); var=string(argv[i]); }
#define tryAlg(str,fun) if (algorithm==str) bestFound = fun();

void parseArgs(int argc, char* argv[]) {
	string eP  = "Parsing the arguments.\n";
	eP += "Arguments must be bin/solver [-<cmd> <value>] where:\n";
	eP += "  -alg    Algorithm to run. Options: GGA, SGA, CHC,\n";
	eP += "          PSO, PBIL, PBILwithHUX and FarEnemyVoronoi.\n";
	eP += "  -f      File from where the data will be loaded.\n";
	eP += "  -10fcv  Index for 10-fcv [0,9]. If not indicated, it uses\n";
	eP += "          the entire dataset for training.\n";
	eP += "  -iter   Maximum number of iterations (default 10000).\n";
	eP += "  -seed   An unsigned integer value to be used as seed by the\n";
	eP += "          pseudo-random number generator (default time(NULL)).\n";
	eP += "  -pop    Size of the population (default 50).\n";
	eP += "  -alpha  Importance of error over reducion for the calculation\n";
	eP += "          of the fitness function (default 0.5).\n";
	eP += "  -bit    Probability of appearance for each bit (default 0.05).\n";
	eP += "  -init   Policy of inicialization for the population, options:\n";
	eP += "          Random (default), ClosestEnemy, FarthestEnemy,\n";
	eP += "          and FarEnemyVoronoi.\n";
	eP += "  -mp     Mutation probability (default 0.001).\n";
	eP += "  -ms     Mutation shift (Only PBIL) (default 0.01).\n";
	eP += "  -lr     Learning Rate (Only PBIL) (default 0.2).\n";
	eP += "  -nlr    Negative Learning Rate (Only PBIL) (default 0.075).\n";
	eP += "  -pso    PSO particles (default 15).\n";
	eP += "  -Vmax   Maximum value for PSO velocity (Only PSO) (default 0.05).\n";
	eP += "  -c1     Weight for local best  (Only PSO) (default 0.2).\n";
	eP += "  -c2     Weight for global best (Only PSO) (default 0.2).\n";
	eP += "  -w      Inertia weight (Only PSO) (default 0.6).\n";
	eP += "  -cross  Crossover probability (default 1.0).\n";
	eP += "  -ex     \% of Closest Enemy instances for the FarEnemyVoronoi\n";
	eP += "          algorithm (default 0.33333).";
	for (int i=1 ; i<argc ; i++) {
		if (argv[i][0]=='-') {
			if (strlen(argv[i])<2)
				FatalError(eP);

			string cmd(&argv[i][1]);
			ifs("alg",algorithm)
			else ifs("f",filepath)
			else ifi("10fcv",tenfcv)
			else ifi("iter",MAX_ITER)
			else ifi("seed",RUN_SEED)
			else ifi("pop",POP_SIZE)
			else iff("alpha",ALPHA)
			else ifs("init",INIT_TYPE)
			else iff("bit",BIT_PROB)
			else iff("mp",MUT_PROB)
			else iff("ms",MS)
			else iff("lr",LR)
			else iff("nlr",NLR)
			else ifi("pso",PARTICLES)
			else iff("vmax",Vmax)
			else iff("c1",C1)
			else iff("c2",C2)
			else iff("w",INERTIA)
			else iff("cross",CROSS_PROB)
			else iff("ex",EXCLUDE)
			else if (cmd=="nv")
				verbose = false;
			else FatalError(eP);
		} else FatalError(eP);
	}
}

int main(int argc, char* argv[]) {

	RUN_SEED = time(NULL);
	parseArgs(argc,argv);
	srand(RUN_SEED);
	if (verbose) printf("Seed:%d\n",RUN_SEED);

	if (tenfcv<-1 || 9<tenfcv)
		FatalError("The index for 10-fcv must be in the range [0,9].");

	clock_t start = clock(), end;
	LoadData(filepath,tenfcv);
	CalcDist();

	Chromosome bestFound;

	tryAlg("PBILwithHUX",PBILwithHUX)
	else tryAlg("PBIL",PBIL)
	else tryAlg("PSO",PSO)
	else tryAlg("GGA",GGA)
	else tryAlg("SGA",SGA)
	else tryAlg("CHC",CHC)
	else tryAlg("ClosestEnemy",ClosestEnemy)
	else tryAlg("FarthestEnemy",FarthestEnemy)
	else tryAlg("FarEnemyVoronoi",FarEnemyVoronoi)
	else tryAlg("CNN",CNN)
	else FatalError("The algorithm specified does not exists.");

	end = clock();
	NN.useJust(bestFound);

	if (verbose) {
		printf("------------------------\n");
		printf("Reduction        %6.2lf%%\n", 100.0-100.0*(double)bestFound.size()/TR.N);
		printf("Training Error   %6.2lf%%\n", 100.0*NN.errorTR());
		if (tenfcv!=-1)
			printf("Test Error       %6.2lf%%\n", 100.0*NN.errorTS());
		printf("Elapsed time     %6.2lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
		printf("------------------------\n");
		bestFound.print();
	} else {
		printf("%.2lf\t",(double)(end-start)/CLOCKS_PER_SEC);
		printf("%.2lf\t", 100.0-100.0*(double)bestFound.size()/TR.N);
		printf("%.2lf\t", 100.0*NN.errorTR());
		printf("%.2lf\n", 100.0*NN.errorTS());
	}

	return 0;
}
