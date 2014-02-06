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

int main(int argc, char* argv[]) {

	if (argc!=4)
		FatalError("Wrong # of arguments. Must be <algorithm> <dataset> <# 10-fcv set>.");

	int tenfcv = atoi(argv[3]);
	if (tenfcv<0 || 9<tenfcv)
		FatalError("The index for 10-fcv must be in the range [0,9].");

	srand(time(NULL));

	LoadData(string(argv[2]),tenfcv);
	NN.CalcDist();
	
	Chromosome bestFound;

	string algoStr(argv[1]);
	if (algoStr == "StandardPBIL") {
		bestFound = StandardPBIL();
	} else if (algoStr == "RandomPBIL") {
		bestFound = RandomPBIL();
	} else if (algoStr == "ClosestEnemyPBIL") {
		bestFound = ClosestEnemyPBIL();
	} else if (algoStr == "FarthestEnemyPBIL") {
		bestFound = FarthestEnemyPBIL();
	} else if (algoStr == "PBILwithHUX") {
		bestFound = PBILwithHUX();
	} else if (algoStr == "PSO") {
		bestFound = PSO();
	} else if (algoStr == "GGA") {
		bestFound = GGA();
	} else if (algoStr == "SGA") {
		bestFound = SGA();
	} else if (algoStr == "CHC") {
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
