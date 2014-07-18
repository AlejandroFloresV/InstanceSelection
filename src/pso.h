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
		int bitsOn = TR.N / PARTICLES;
		double hi_prob = min(0.9, BIT_PROB * TR.N * 0.7 / ((double)bitsOn));
		double low_prob = (BIT_PROB * TR.N - bitsOn * hi_prob)/((double)(TR.N - bitsOn));
		X = vector<double>(TR.N,low_prob);
		V = vector<double>(TR.N);
		for (int i=0 ; i<TR.N ; i++) {
			if (ind == pso_partition[i])
				X[i] = hi_prob;
			V[i] = drand()*2*Vmax - Vmax;
		}
	}

	void GenerateSamples();
	void UpdateParticle();
};

void Particle::GenerateSamples() {

	Population pop(POP_SIZE);
	vector<thread> t;
	for (int p=0 ; p<POP_SIZE ; p++) {
		pop[p] = Chromosome(ref(X));
		t.push_back(thread(calc_fit_chromosome,ref(pop[p])));
	}
	for (int i=0 ; i<t.size() ; i++)
		t[i].join();
	sortPopulation(pop);

	// Update Global Best
	if (Iter==0 || pop[0] < LBest) {
		LBest = pop[0];
		if (LBest < *GBest)
			*GBest = LBest;
	}
	Iter++;
}

void Particle::UpdateParticle() {
	bool li,gi;
	double dC1 = C1*drand(), dC2 = C2*drand();
	for (int i=0 ; i<TR.N ; i++) {
		li = LBest[i];
		gi = (&GBest)[i];
		V[i] = INERTIA*V[i] + dC1*(b2d(li)-X[i]) + dC2*(b2d(gi)-X[i]);
		V[i] = max(-Vmax,min(Vmax,V[i]));
		X[i] = max(0.0,min(1.0,X[i]+V[i]));
	}
}

// +---------------+
// | PSO ALGORITHM |
// +---------------+

Chromosome PSO() {
	Chromosome bestC((int)(BIT_PROB*TR.N));
	
	pso_partition.clear();
	for (int i=0 ; i<TR.N ; i++)
		pso_partition.push_back(rand() % PARTICLES);

	vector<Particle> p(PARTICLES);
	for (int j=0 ; j<PARTICLES ; j++)
		p[j] = Particle(&bestC,j);
	
	for (int i=0 ; i<MAX_ITER ; i++) {
		for (int j=0 ; j<PARTICLES ; j++) {
			p[j].GenerateSamples();
			p[j].UpdateParticle();
		}
	}
	return bestC;
}
