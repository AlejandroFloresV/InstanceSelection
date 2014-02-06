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

	vector<double> Vp;
	int size,Iter;
	double MP,MS,LR,NLR;
	Chromosome GBestC;
	Population pop;

	public:
	ProbVector() {
		MP = 0.02;
		MS = 0.01;
		LR = 0.2;
		NLR = 0.075;
		size = 50;
		Iter = 0;
		Vp = vector<double>(TR.N,0.05);
		pop = Population(size);
	}
	void setSize(int s) { size = s; }
	void setMP(double mp) { MP = mp; }
	void setMS(double ms) { MS = ms; }
	void setLR(double lr) { LR = lr; }
	void setNLR(double nlr) { NLR = nlr; }

	void RandomProb();
	void EnemyProb(bool);
	void ClosestEnemyProb();
	void FarthestEnemyProb();
	void GenerateSamples();
	void GenerateSamplesHUX();
	void UpdateVp();
	Chromosome GetBest() { return GBestC; }
};

void ProbVector::RandomProb() {
	for (int i=0 ; i<TR.N ; i++)
		Vp[i] = 0.03 + drand()*0.04;
}

void ProbVector::EnemyProb(bool closest) {

	Vp = vector<double>(TR.N,0.02);
	vector<double> cd = NN.ClosestEnemy();
	vector<pair<double,int> > cp(TR.N);

	for (int i=0 ; i<TR.N ; i++)
		cp[i] = make_pair(cd[i],i);
	sort(cp.begin(),cp.end());

	for (int i=(closest ? TR.N-1 : 0), j=TR.N/40 ;
		i>=0 && j>0 ; (closest ? i-- : i++), j--)
		Vp[cp[i].second] = 0.9;
}

void ProbVector::ClosestEnemyProb() {
	EnemyProb(true);
}

void ProbVector::FarthestEnemyProb() {
	EnemyProb(false);
}

void ProbVector::GenerateSamples() {

	for (int p=0 ; p<size ; p++) {
		vector<int> currV;
		for (int i=0 ; i<TR.N ; i++) {
			if (drand()<Vp[i])
				currV.push_back(i);
		}
		pop[p] = Chromosome(currV);
	}

	sortPopulation(pop);

	// Update Global Best
	if (Iter==0 || pop[0].fitness() < GBestC.fitness())
		GBestC = pop[0];
	Iter++;
}

void ProbVector::GenerateSamplesHUX() {

	int s = size/2, p, iA, iB;
	if (s%2!=0) s++;
	for (p=0 ; p<s ; p++) {
		vector<int> currV;
		for (int i=0 ; i<TR.N ; i++) {
			if (drand()<Vp[i])
				currV.push_back(i);
		}
		pop[p] = Chromosome(currV);
	}
	for (; p<size ; p+=2) {
		iA = rand() % s;
		iB = rand() % s;
		Offspring crossAB = CrossoverHUX(pop[iA],pop[iB]);
		pop[p] = crossAB.first;
		pop[p+1] = crossAB.second;
	}

	sortPopulation(pop);

	// Update Global Best
	if (Iter==0 || pop[0].fitness() < GBestC.fitness())
		GBestC = pop[0];
	Iter++;
}

void ProbVector::UpdateVp() {

	Chromosome
		*LBestC = &pop[0],
		*LWorstC = &pop[size-1];
	(*LBestC).iterator();
	(*LWorstC).iterator();
	bool bi,wi;
	for (int i=0 ; i<TR.N ; i++) {
		bi = (*LBestC).next();
		wi = (*LWorstC).next();
		Vp[i] = Vp[i]*(1.0-LR) + b2i(bi)*LR;
		if (bi!=wi)
			Vp[i] = Vp[i]*(1.0-NLR) + b2i(bi)*NLR;

		// Mutation
		if (drand() < MP)
			Vp[i] = Vp[i]*(1.0-MS) + ((double)(rand()%2))*MS;
	}
}

// +-------------------------+
// | PBIL ALGORITHM VERSIONS |
// +-------------------------+

#define PBIL_MAXITER 1000

Chromosome StandardPBIL() {
	ProbVector vp;
	for (int i=0 ; i<PBIL_MAXITER ; i++) {
		vp.GenerateSamples();
		vp.UpdateVp();
	}
	return vp.GetBest();
}

Chromosome RandomPBIL() {
	ProbVector vp;
	vp.RandomProb();
	for (int i=0 ; i<PBIL_MAXITER ; i++) {
		vp.GenerateSamples();
		vp.UpdateVp();
	}
	return vp.GetBest();
}

Chromosome ClosestEnemyPBIL() {
	ProbVector vp;
	vp.ClosestEnemyProb();
	for (int i=0 ; i<PBIL_MAXITER ; i++) {
		vp.GenerateSamples();
		vp.UpdateVp();
	}
	return vp.GetBest();
}

Chromosome FarthestEnemyPBIL() {
	ProbVector vp;
	vp.FarthestEnemyProb();
	for (int i=0 ; i<PBIL_MAXITER ; i++) {
		vp.GenerateSamples();
		vp.UpdateVp();
	}
	return vp.GetBest();
}

Chromosome PBILwithHUX() {
	ProbVector vp;
	for (int i=0 ; i<PBIL_MAXITER ; i++) {
		vp.GenerateSamplesHUX();
		vp.UpdateVp();
	}
	return vp.GetBest();
}
