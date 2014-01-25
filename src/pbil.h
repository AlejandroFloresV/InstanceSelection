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
	int Pop,Iter;
	double MP,MS,LR,NLR;
	Chromosome GBestC,LBestC,LWorstC;
	double GBestF;

	public:
	ProbVector() {
		MP = 0.02;
		MS = 0.01;
		LR = 0.2;
		NLR = 0.075;
		Pop = 50;
		Iter = 0;
		GBestF = -1.0;
		Vp = vector<double>(TR.N,0.05);
	}
	void setPop(int p) { Pop = p; }
	void setMP(double mp) { MP = mp; }
	void setMS(double ms) { MS = ms; }
	void setLR(double lr) { LR = lr; }
	void setNLR(double nlr) { NLR = nlr; }

	void RandomProb();
	void EnemyProb(bool);
	void ClosestEnemyProb();
	void FarthestEnemyProb();
	void GenerateSamples();
	void UpdateVp();
	Chromosome GetBest() { return GBestC; }
};

void ProbVector::RandomProb() {
	for (int i=0 ; i<TR.N ; i++)
		Vp[i] = 0.03 + drand()*0.04;
}

void ProbVector::EnemyProb(bool closest) {

	Vp = vector<double>(TR.N,0.03);
	vector<double> cd = NN.ClosestEnemy();
	vector<pair<double,int> > cp(TR.N);

	for (int i=0 ; i<TR.N ; i++)
		cp[i] = make_pair(cd[i],i);
	sort(cp.begin(),cp.end());

	for (int i=(closest ? TR.N-1 : 0), j=TR.N/25 ;
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

	Iter++;
	vector<Chromosome> newPop(Pop);
	for (int p=0 ; p<Pop ; p++) {
		vector<int> currV;
		for (int i=0 ; i<TR.N ; i++) {
			if (drand()<Vp[i])
				currV.push_back(i);
		}
		newPop[p] = Chromosome(currV);
	}

	sort(newPop.begin(),newPop.end());
	LBestC  = newPop[0];
	LWorstC = newPop[Pop-1];

	// Update Global Best
	if (GBestF < 0.0 || LBestC.fitness() < GBestF) {
		GBestC = LBestC;
		//cout << "Found Local Min " << GBestF << " at iteration " << thisIter << endl;
	}
}

void ProbVector::UpdateVp() {

	LBestC.iterator();
	LWorstC.iterator();
	bool bi,wi;
	for (int i=0 ; i<TR.N ; i++) {
		bi = LBestC.next();
		wi = LWorstC.next();
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
