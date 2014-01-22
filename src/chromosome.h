#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

#define MUTATION_RATE 0.001

class Chromosome {
	int it,valIt;
	mutable double fit;
	public:
	int on,n;
	vector<int> gene;
	Chromosome() {}
	Chromosome(int,int);
	Chromosome(int,vector<int>,double);
	double fitness() const;
	void iterator();
	bool next();
	void mutate();
	void print();
	bool operator<(const Chromosome& val) const {
		return fitness() < val.fitness();
	}
};

Chromosome::Chromosome(int _n, int _on) {
	on = _on;
	n = _n;
	fit = -1.0;
	it = valIt = 0;
	set<int> m;
	while (m.size()<_on)
		m.insert(rand()%_n);
	gene = vector<int>(m.begin(),m.end());
}

Chromosome::Chromosome(int _n, vector<int> genesOn, double _fit = -1.0) {
	n = _n;
	fit = _fit;
	it = valIt = 0;
	on = genesOn.size();
	gene = genesOn;
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
	for (int i=0 ; i<n ; i++) {
		isOn = this->next();
		if (drand() < MUTATION_RATE)
			isOn = !isOn;
		if (isOn)
			newGene.push_back(i);
	}
	gene = newGene;
}

void Chromosome::print() {
	printf("Size(%d) On(%d) : ",n,on);
	for (int i=0 ; i<on ; i++)
		printf("%d ",gene[i]);
	printf("\n");
}
