#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

class Chromosome {
	int on, it, valIt;
	mutable double fit;
	vector<int> gene;

	public:
	Chromosome();
	Chromosome(int);
	Chromosome(vector<int>);
	Chromosome(vector<double>);
	void calc_fitness();
	double fitness() const;
	void iterator();
	bool next();
	void mutate();
	void print();
	int size() { return on; }
	vector<int> get() { return gene; }
	bool operator<(const Chromosome& val) const {
		return fitness() < val.fitness();
	}
};

Chromosome::Chromosome() {
	fit = -1.0;
	on = it = valIt = 0;
}

Chromosome::Chromosome(int _on) {
	on = _on;
	fit = -1.0;
	it = valIt = 0;
	set<int> m;
	while (m.size()<_on)
		m.insert(rand() % TR.N);
	gene = vector<int>(m.begin(),m.end());
}

Chromosome::Chromosome(vector<int> _gene) {
	fit = -1.0;
	it = valIt = 0;
	on = _gene.size();
	gene = _gene;
}

Chromosome::Chromosome(vector<double> vp) {
	fit = -1.0;
	it = on = 0;
	for (int i=0 ; i<TR.N ; i++)
		if (drand() < vp[i]) {
			gene.push_back(i);
			on++;
		}
}

void Chromosome::iterator() {
	it = valIt = 0;
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
	iterator();
	vector<int> newGene;
	bool isOn;
	for (int i=0 ; i<TR.N ; i++) {
		isOn = next();
		if (drand() < MUT_PROB)
			isOn = !isOn;
		if (isOn)
			newGene.push_back(i);
	}
	gene = newGene;
	on = newGene.size();
	fit = -1.0;
}

void Chromosome::print() {
	printf("On(%d) : ",on);
	for (int i=0 ; i<on ; i++)
		printf("%d ",TR.Index[gene[i]]);
	printf("\n");
}


void calc_fit_chromosome(Chromosome &c) {
	c.calc_fitness();
}
