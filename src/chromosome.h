#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

class Chromosome {
	int it,valIt;
	mutable double fit;
	public:
	int on;
	vector<int> gene;
	Chromosome() {}
	Chromosome(int);
	Chromosome(vector<int>,double);
	double fitness() const;
	void iterator();
	bool next();
	void mutate();
	void print();
	bool operator<(const Chromosome& val) const {
		return fitness() < val.fitness();
	}
};

Chromosome::Chromosome(int _on) {
	on = _on;
	fit = -1.0;
	it = valIt = 0;
	set<int> m;
	while (m.size()<_on)
		m.insert(rand() % TR.N);
	gene = vector<int>(m.begin(),m.end());
}

Chromosome::Chromosome(vector<int> genesOn, double _fit = -1.0) {
	fit = _fit;
	it = valIt = 0;
	on = genesOn.size();
	gene = genesOn;
}

double Chromosome::fitness() const {
	if (fit < 0.0) {
		NN.useJust(gene);
		fit = NN.fitnessAR();
	}
	return fit;
}

void Chromosome::iterator() {
	valIt = it = 0;
}

bool Chromosome::next() {
	if (on <= it)
		return false;
	if (valIt++ == gene[it]) {
		it++;
		return true;
	}
	return false;
}

void Chromosome::mutate() {
	this->iterator();
	vector<int> newGene;
	bool isOn;
	for (int i=0 ; i<TR.N ; i++) {
		isOn = this->next();
		if (drand() < MUT_PROB)
			isOn = !isOn;
		if (isOn)
			newGene.push_back(i);
	}
	this->gene = newGene;
	this->on = newGene.size();
}

void Chromosome::print() {
	printf("On(%d) : ",on);
	for (int i=0 ; i<on ; i++)
		printf("%d ",TR.Index[gene[i]]);
	printf("\n");
}
