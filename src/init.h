#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<cmath>

using namespace std;

#define tryInit(str,fun) if (INIT_TYPE==str) s = fun();
#define Population vector<Chromosome>

Chromosome initSolEnemy(bool closest) {
	vector<pair<double,int> > cp = SortedEnemyDistance();
	Chromosome sol;
	for (int i=(closest ? 0 : TR.N-1), j=TR.N*BIT_PROB
		; j>=0 ; (closest ? i++ : i--), j--)
		sol.set(cp[i].second);
	return sol;
}

#define ClosestNE() initSolEnemy(true)
#define FarthestNE() initSolEnemy(false)

Chromosome initSolution() {
	Chromosome s;
	tryInit("ClosestNE",ClosestNE)
	else tryInit("FarthestNE",FarthestNE)
	else tryInit("NEHS",NEHS)
	else tryInit("CNN",CNN)
	else FatalError("Wrong type of population initialization.");
	return s;
}

vector<double> initProbVector() {
	if (INIT_TYPE == "Random")
		return vector<double>(TR.N,BIT_PROB);

	Chromosome init = initSolution();
	int bitsOn = init.size();
	double hi_prob = min(0.9,BIT_PROB * TR.N * 0.7 / ((double)bitsOn));
	double low_prob = (BIT_PROB * TR.N - bitsOn * hi_prob)/((double)(TR.N - bitsOn));
	vector<double> vp(TR.N,low_prob);
	for (int i=0 ; i<TR.N ; i++)
		if (init[i])
			vp[i] = hi_prob;
	return vp;
}

Population initPopulation() {
	Population pop(POP_SIZE);
	vector<double> vp = initProbVector();
	for (int i=0 ; i<POP_SIZE ; i++)
		pop[i] = Chromosome(vp);
	return pop;
}
