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
	Chromosome *GBestC,LBestC;
	double *GBestF,LBestF,c1,c2;

	public:
	Particle() {}
	Particle(Chromosome *gbc, double *gbf, int ind) {
		GBestC = gbc;
		GBestF = gbf;
		(*GBestF) = LBestF = -1.0;
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
	double currF, bestF;
	vector<int> bestV;

	for (int p=0 ; p<POP_SIZE ; p++) {
		vector<int> currV;
		for (int i=0 ; i<TR.N ; i++) {
			if (drand()<X[i])
				currV.push_back(i);
		}
		NN.useJust(currV);
		currF = NN.fitnessAR();
		if (p==0 || currF < bestF) {
			bestF = currF;
			bestV = currV;
		}
	}

	// Update Local & Global Best
	if (LBestF<0.0 || bestF < LBestF) {
		LBestF = bestF;
		LBestC = Chromosome(bestV);
		if (*GBestF<0.0 || LBestF < *GBestF) {
			*GBestF = LBestF;
			*GBestC = LBestC;
			//cout << "Found Local Min " << *GBestF << endl;
		}
	}
}

void Particle::UpdateParticle(double w) {
	LBestC.iterator();
	GBestC->iterator();
	bool li,gi;
	for (int i=0 ; i<TR.N ; i++) {
		li = LBestC.next();
		gi = GBestC->next();
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
	double bestF,w;
	
	pso_partition.clear();
	for (int i=0 ; i<TR.N ; i++)
		pso_partition.push_back(rand() % PARTICLES);

	vector<Particle> p(PARTICLES);
	for (int j=0 ; j<PARTICLES ; j++)
		p[j] = Particle(&bestC,&bestF,j);
	
	for (int i=0 ; i<MAX_ITER ; i++) {
		for (int j=0 ; j<PARTICLES ; j++) {
			p[j].GenerateSamples();
			p[j].UpdateParticle(w);
			w = 1.5*((double)(MAX_ITER-i-1))/((double)MAX_ITER+1.0);
		}
	}
	return bestC;
}
