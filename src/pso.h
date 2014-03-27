#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

// +--------------+
// | PSO PARTICLE |
// +--------------+

vector<int> pso_partition;

class Particle {
	
	vector<double> V,X;
	Chromosome *GBest,LBest;
	int Iter;

	public:
	Particle() {}
	Particle(Chromosome *gbc, int ind) {
		GBest = gbc;
		Iter = 0;
		X = vector<double>(TR.N);
		V = vector<double>(TR.N);
		for (int i=0 ; i<TR.N ; i++) {
			if (ind == pso_partition[i])
				X[i] = 0.4 + drand()*0.1;
			else X[i] = 0.05 + drand()*0.05;
			V[i] = drand()*0.06-0.03;
		}
	}

	void GenerateSamples();
	void UpdateParticle(double);
};

void Particle::GenerateSamples() {

	Chromosome bc, cc;
	for (int p=0 ; p<POP_SIZE ; p++) {
		cc = Chromosome(V);
		if (p==0 || cc < bc)
			bc = cc;
	}

	// Update Global Best
	if (Iter==0 || bc < LBest) {
		LBest = bc;
		if (Iter==0 || bc < *GBest)
			*GBest = bc;
	}
	Iter++;
}

void Particle::UpdateParticle(double w) {
	LBest.iterator();
	GBest->iterator();
	bool li,gi;
	for (int i=0 ; i<TR.N ; i++) {
		li = LBest.next();
		gi = GBest->next();
		V[i] = w*V[i] + C1*drand()*(b2d(li)-X[i]) + C2*drand()*(b2d(gi)-X[i]);
		V[i] = max(-0.1,min(0.1,V[i]));
		X[i] = max(0.0,min(1.0,X[i]+V[i]));
	}
}

// +---------------+
// | PSO ALGORITHM |
// +---------------+

Chromosome PSO() {
	Chromosome bestC;
	double w;
	
	pso_partition.clear();
	for (int i=0 ; i<TR.N ; i++)
		pso_partition.push_back(rand() % PARTICLES);

	vector<Particle> p(PARTICLES);
	for (int j=0 ; j<PARTICLES ; j++)
		p[j] = Particle(&bestC,j);
	
	for (int i=0 ; i<MAX_ITER ; i++) {
		for (int j=0 ; j<PARTICLES ; j++) {
			p[j].GenerateSamples();
			p[j].UpdateParticle(w);
			w = 1.5*((double)(MAX_ITER-i-1))/((double)MAX_ITER+1.0);
		}
	}
	return bestC;
}
