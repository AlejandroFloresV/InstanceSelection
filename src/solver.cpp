#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>
#include "conf.h"
#include "tenfold.h"
#include "onenn.h"
#include "chromosome.h"
#include "genetic.h"
#include "pbil.h"
#include "pso.h"

using namespace std;

string algorithm, filepath;
int tenfcv = 0;

#define ifi(str,var) if (cmd==str) { i++; if (i==argc) FatalError(eP); var=atoi(argv[i]); }
#define iff(str,var) if (cmd==str) { i++; if (i==argc) FatalError(eP); var=atof(argv[i]); }
#define ifs(str,var) if (cmd==str) { i++; if (i==argc) FatalError(eP); var=string(argv[i]); }
#define tryAlg(str,fun) if (algorithm==str) bestFound = fun();

void parseArgs(int argc, char* argv[]) {
	string eP  = "Parsing the arguments.\n";
	eP += "Arguments must be bin/solver [-<cmd> <value>] where:\n";
	eP += "  -alg   Algorithm to run. Options:\n";
	eP += "         GGA, SGA, CHC, PBIL and PSO.\n";
	eP += "  -f     File from where the data will be loaded.\n";
	eP += "  -10fcv Index for 10-fcv [0,9] (default 0).\n";
	eP += "  -iter  Maximum number of iterations (default 10000).\n";
	eP += "  -pop   Size of the population (default 50).\n";
	eP += "  -init  Policy of inicialization for the population, options:\n";
	eP += "         Random (default), ClosestEnemy, FarthestEnemy.\n";
	eP += "  -mp    Mutation probability (default 0.001).\n";
	eP += "  -ms    Mutation shift (Only PBIL) (default 0.01).\n";
	eP += "  -lr    Learning Rate (Only PBIL) (default 0.2).\n";
	eP += "  -nlr   Negative Learning Rate (Only PBIL) (default 0.075).\n";
	eP += "  -pso   PSO particles (default 15).\n";
	eP += "  -c1    Weight for local best (Only PSO) (default 3.0).\n";
	eP += "  -c2    Weight for global best (Only PSO) (default 1.0).\n";
	eP += "  -cross Crossover probability (default 1.0).";
	for (int i=1 ; i<argc ; i++) {
		if (argv[i][0]=='-') {
			if (strlen(argv[i])<2)
				FatalError(eP);

			string cmd(&argv[i][1]);
			ifs("alg",algorithm)
			else ifs("f",filepath)
			else ifi("10fcv",tenfcv)
			else ifi("iter",MAX_ITER)
			else ifi("pop",POP_SIZE)
			else ifs("init",INIT_TYPE)
			else iff("mp",MUT_PROB)
			else iff("ms",MS)
			else iff("lr",LR)
			else iff("nlr",NLR)
			else ifi("pso",PARTICLES)
			else iff("c1",C1)
			else iff("c2",C2)
			else iff("cross",CROSS_PROB)
			else if (cmd=="nv")
				verbose = false;
			else FatalError(eP);
		} else FatalError(eP);
	}
}

int main(int argc, char* argv[]) {

	parseArgs(argc,argv);

	if (tenfcv<0 || 9<tenfcv)
		FatalError("The index for 10-fcv must be in the range [0,9].");

	srand(time(NULL));

	LoadData(filepath,tenfcv);
	NN.CalcDist();
	
	Chromosome bestFound;

	tryAlg("PBILwithHUX",PBILwithHUX)
	else tryAlg("PBIL",PBIL)
	else tryAlg("PSO",PSO)
	else tryAlg("GGA",GGA)
	else tryAlg("SGA",SGA)
	else tryAlg("CHC",CHC)
	else FatalError("The algorithm specified does not exists.");

	NN.useJust(bestFound.gene);

	if (verbose) {
		printf("----------------------\n");
		printf("Reduction      %6.2lf%%\n", 100.0-100.0*(double)bestFound.on/TR.N);
		printf("Training Error %6.2lf%%\n", 100.0*NN.errorTR());
		printf("Test Error     %6.2lf%%\n", 100.0*NN.errorTS());
		printf("----------------------\n");
		bestFound.print();
	} else {
		printf("%.2lf\n", 100.0-100.0*(double)bestFound.on/TR.N);
		printf("%.2lf\n", 100.0*NN.errorTR());
		printf("%.2lf\n", 100.0*NN.errorTS());
	}

	return 0;
}
