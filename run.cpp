#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>
#include "conf.h"
#include "chromosome.h"
#include "tenfold.h"
#include "onenn.h"
#include "pbil.h"
#include "pso.h"
#include "genetic.h"

using namespace std;

int main(int argc, char* argv[]) {

	if (argc!=4)
		FatalError("Wrong # of arguments. Must be <dataset> <# 10-fcv set>.");

	int tenfcv = atoi(argv[3]);
	if (tenfcv<0 || 9<tenfcv)
		FatalError("The index for 10-fcv must be in the range [0,9].");

	srand(time(NULL));

	LoadData(string(argv[2]),tenfcv);
	NN.CalcDist();
	
	Chromosome bestFound;

	string algoStr(argv[1]);
	if (algoStr.compare("StandardPBIL")==0) {
		bestFound = StandardPBIL();
	} else if (algoStr.compare("RandomPBIL")==0) {
		bestFound = RandomPBIL();
	} else if (algoStr.compare("ClosestEnemyPBIL")==0) {
		bestFound = ClosestEnemyPBIL();
	} else if (algoStr.compare("PSO")==0) {
		bestFound = PSO();
	} else if (algoStr.compare("GGA")==0) {
		bestFound = GGA();
	} else if (algoStr.compare("SGA")==0) {
		bestFound = SGA();
	} else if (algoStr.compare("CHC")==0) {
		bestFound = CHC();
	} else {
		FatalError("The name of the algorithm argument is wrong.");
	}

	NN.useJust(bestFound.gene);

	printf("%.2lf\n", 100.0-100.0*(double)bestFound.on/TR.N);
	printf("%.2lf\n", 100.0*NN.errorTR());
	printf("%.2lf\n", 100.0*NN.errorTS());

	//printf("----------------------\n");
	//printf("Reduction      %6.2lf%%\n", 100.0-100.0*(double)bestFound.on/TR.N);
	//printf("Training Error %6.2lf%%\n", 100.0*NN.errorTR());
	//printf("Test Error     %6.2lf%%\n", 100.0*NN.errorTS());
	//printf("----------------------\n");
	//bestFound.print();
	
	return 0;
}
