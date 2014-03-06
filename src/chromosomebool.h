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
	vector<bool> gene;

	public:
	Chromosome();
	Chromosome(int);
	Chromosome(vector<int>);
	Chromosome(vector<double>);
	double fitness() const;
	void iterator();
	bool next();
	void mutate();
	void print();
	int size() { return on; }
	vector<int> get() {
		vector<int> geneOn;
		for (int i=0 ; i<TR.N ; i++)
			if (gene[i])
				geneOn.push_back(i);
		return geneOn;
	}
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
	gene = vector<bool>(TR.N,false);
	for (set<int>::iterator its=m.begin(); its!=m.end(); ++its)
		gene[*its] = true;
}

Chromosome::Chromosome(vector<int> _gene) {
	fit = -1.0;
	it = valIt = 0;
	on = _gene.size();
	gene = vector<bool>(TR.N,false);
	for (vector<int>::iterator its=_gene.begin(); its!=_gene.end(); ++its)
		gene[*its] = true;
}

Chromosome::Chromosome(vector<double> vp) {
	fit = -1.0;
	it = on = 0;
	gene = vector<bool>(TR.N,false);
	for (int i=0 ; i<TR.N ; i++)
		if (drand() < vp[i]) {
			gene[i] = true;
			on++;
		}
}

void Chromosome::iterator() {
	it = 0;
}

bool Chromosome::next() {
	if (TR.N <= it)
		return false;
	return gene[it++];
}

void Chromosome::mutate() {
	for (int i=0 ; i<TR.N ; i++) {
		if (drand() < MUT_PROB) {
			gene[i] = !gene[i];
			if (gene[i]) on++;
			else on--;
		}
	}
	fit = -1.0;
}

void Chromosome::print() {
	printf("On(%d) : ",on);
	for (int i=0 ; i<TR.N ; i++)
		if (gene[i])
			printf("%d ",TR.Index[i]);
	printf("\n");
}
