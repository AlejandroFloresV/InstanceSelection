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

class Particle {
	
	vector<double> V,X;
	Chromosome *GBest,LBest;
	int Iter;

	public:
	Particle() {}
	Particle(Chromosome *gbc) {
		GBest = gbc;
		Iter = 0;
		X = initProbVector();
		V = vector<double>(TR.N);
		for (int i=0 ; i<TR.N ; i++)
			V[i] = drand()*2.0*Vmax - Vmax;
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
		if (LBest < *GBest) {
			*GBest = LBest;
			LAST_BEST_ITER = Iter+1;
		}
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

	vector<Particle> p(PARTICLES);
	for (int j=0 ; j<PARTICLES ; j++)
		p[j] = Particle(&bestC);
	
	for (int i=0 ; i<MAX_ITER ; i++) {
		for (int j=0 ; j<PARTICLES ; j++) {
			p[j].GenerateSamples();
			p[j].UpdateParticle();
		}
	}
	return bestC;
}
