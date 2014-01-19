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
	int Pop;
	Chromosome *GBestC,LBestC;
	double *GBestF,LBestF,c1,c2;

	public:
	Particle() {}
	Particle(Chromosome *gbc, double *gbf, int ind) {
		Pop = 40;
		GBestC = gbc;
		GBestF = gbf;
		c1 = 3.0;
		c2 = 1.0;
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
	void setPop(int p) { Pop = p; }
	void setC1(int c) { c1 = c; }
	void setC2(int c) { c2 = c; }

	void GenerateSamples();
	void UpdateParticle(double);
};

void Particle::GenerateSamples() {
	double currF, bestF;
	vector<int> bestV;

	for (int p=0 ; p<Pop ; p++) {
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
		LBestC = Chromosome(TR.N,bestV);
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
		V[i] = w*V[i] + c1*drand()*(b2d(li)-X[i]) + c2*drand()*(b2d(gi)-X[i]);
		V[i] = max(-0.1,min(0.1,V[i]));
		X[i] = max(0.0,min(1.0,X[i]+V[i]));
	}
}

// +---------------+
// | PSO ALGORITHM |
// +---------------+

#define PART_PSO 15
#define ITER_PSO 250

Chromosome PSO() {
	Chromosome bestC;
	double bestF,w;
	
	pso_partition.clear();
	for (int i=0 ; i<TR.N ; i++)
		pso_partition.push_back(rand()%PART_PSO);

	Particle p[PART_PSO];
	for (int j=0 ; j<PART_PSO ; j++)
		p[j] = Particle(&bestC,&bestF,j);
	
	for (int i=0 ; i<ITER_PSO ; i++) {
		for (int j=0 ; j<PART_PSO ; j++) {
			p[j].GenerateSamples();
			p[j].UpdateParticle(w);
			w = 1.5*((double)(ITER_PSO-i-1))/((double)ITER_PSO+1.0);
		}
	}
	return bestC;
}
