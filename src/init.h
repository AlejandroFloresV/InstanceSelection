#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<cmath>

using namespace std;

#define Population vector<Chromosome>

Chromosome initSolEnemy(bool closest) {

	vector<double> ed = NN.EnemyDistance();
	vector<pair<double,int> > cp(TR.N);

	for (int i=0 ; i<TR.N ; i++)
		cp[i] = make_pair(ed[i],i);
	sort(cp.begin(),cp.end());

	vector<int> v;
	for (int i=(closest ? 0 : TR.N-1), j=TR.N*BIT_PROB
		; j>=0 ; (closest ? i++ : i--), j--)
		v.push_back(cp[i].second);
	sort(v.begin(),v.end());

	return Chromosome(v);
}

Chromosome initSolution() {
	Chromosome s;
	if (INIT_TYPE == "ClosestEnemy")
		s = initSolEnemy(true);
	else if (INIT_TYPE == "FarthestEnemy")
		s = initSolEnemy(false);
	else if (INIT_TYPE == "FarEnemyVoronoi")
		s = FarEnemyVoronoi();
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
	init.iterator();
	for (int i=0 ; i<TR.N ; i++)
		if (init.next())
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
