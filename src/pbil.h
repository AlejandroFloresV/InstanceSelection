#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

// +--------------------+
// | PROBABILITY VECTOR |
// +--------------------+

class ProbVector {

	int Iter;
	Chromosome GBestC;
	Population pop;
	vector<double> Vp;

	public:
	ProbVector() {
		Iter = 0;
		pop = Population(POP_SIZE);
		Vp = initProbVector();
	}
	void GenerateSamples();
	void GenerateSamplesHUX();
	void UpdateVp();
	Chromosome GetBest() { return GBestC; }
};

void ProbVector::GenerateSamples() {

	vector<thread> t;
	for (int p=0 ; p<POP_SIZE ; p++) {
		pop[p] = Chromosome(ref(Vp));
		t.push_back(thread(calc_fit_chromosome,ref(pop[p])));
	}
	for (int i=0 ; i<t.size() ; i++)
		t[i].join();
	sortPopulation(pop);

	// Update Global Best
	if (Iter==0 || pop[0] < GBestC)
		GBestC = pop[0];
	Iter++;
}

void ProbVector::GenerateSamplesHUX() {

	int s = POP_SIZE/2, p, iA, iB;
	if (s%2!=0) s++;
	for (p=0 ; p<s ; p++)
		pop[p] = Chromosome(ref(Vp));

	for (; p<POP_SIZE ; p+=2) {
		iA = rand() % s;
		iB = rand() % s;
		Offspring crossAB = CrossoverHUX(pop[iA],pop[iB]);
		pop[p] = crossAB.first;
		pop[p+1] = crossAB.second;
	}

	sortPopulation(pop);

	// Update Global Best
	if (Iter==0 || pop[0] < GBestC)
		GBestC = pop[0];
	Iter++;
}

void ProbVector::UpdateVp() {

	bool bi,wi;
	for (int i=0 ; i<TR.N ; i++) {
		bi = pop[0][i];
		wi = pop[POP_SIZE-1][i];
		Vp[i] = Vp[i]*(1.0-LR) + b2i(bi)*LR;
		if (bi!=wi)
			Vp[i] = Vp[i]*(1.0-NLR) + b2i(bi)*NLR;

		// Mutation
		if (drand() < MUT_PROB)
			Vp[i] = Vp[i]*(1.0-MS) + ((double)(rand()%2))*MS;
	}
}

// +----------------+
// | PBIL ALGORITHM |
// +----------------+

Chromosome PBIL() {
	ProbVector vp;
	for (int i=0 ; i<MAX_ITER ; i++) {
		vp.GenerateSamples();
		vp.UpdateVp();
	}
	return vp.GetBest();
}

Chromosome PBILwithHUX() {
	ProbVector vp;
	for (int i=0 ; i<MAX_ITER ; i++) {
		vp.GenerateSamplesHUX();
		vp.UpdateVp();
	}
	return vp.GetBest();
}
