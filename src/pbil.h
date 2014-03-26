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

		if (INIT_TYPE == "Random")
			Vp = vector<double>(TR.N,0.05);
		else if (INIT_TYPE == "ClosestEnemy")
			this->EnemyProb(true);
		else if (INIT_TYPE == "FarthestEnemy")
			this->EnemyProb(false);
		else if (INIT_TYPE == "FarEnemyVoronoi")
			this->ProbFarEnemyVoronoi();
		else FatalError("Wrong type of population initialization.");
	}

	void EnemyProb(bool);
	void ProbFarEnemyVoronoi();
	void GenerateSamples();
	void GenerateSamplesHUX();
	void UpdateVp();
	Chromosome GetBest() { return GBestC; }
};

void ProbVector::EnemyProb(bool closest) {

	Vp = NN.EnemyDistance();
	vector<pair<double,int> > cp(TR.N);

	for (int i=0 ; i<TR.N ; i++)
		cp[i] = make_pair(Vp[i],i);
	sort(cp.begin(),cp.end());
	Vp = vector<double>(TR.N,0.02);

	for (int i=(closest ? 0 : TR.N-1), j=TR.N/35 ;
		i>=0 && j>=0 ; (closest ? i++ : i--), j--)
		Vp[cp[i].second] = 0.9;
}

void ProbVector::ProbFarEnemyVoronoi() {

	Vp = vector<double>(TR.N,0.02);
	Chromosome initSol = FarEnemyVoronoi();
	initSol.iterator();
	for (int i=0 ; i<TR.N ; i++)
		if (initSol.next())
			Vp[i] = 0.9;
}

void ProbVector::GenerateSamples() {

	for (int p=0 ; p<POP_SIZE ; p++)
		pop[p] = Chromosome(Vp);
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
		pop[p] = Chromosome(Vp);

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

	pop[0].iterator();
	pop[POP_SIZE-1].iterator();
	bool bi,wi;
	for (int i=0 ; i<TR.N ; i++) {
		bi = pop[0].next();
		wi = pop[POP_SIZE-1].next();
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
