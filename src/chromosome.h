#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

class Chromosome {
	int on;
	mutable double fit;
	vector<bool> gene;

	public:
	Chromosome();
	Chromosome(int);
	Chromosome(vector<double>&);
	double fitness() const;
	void calc_fitness();
	void mutate();
	void print();
	int size() { return on; }
	bool operator<(const Chromosome& val) const {
		return fitness() < val.fitness();
	}
	bool operator[](const int& ind) {
		return gene[ind];
	}
	void set(int i, bool v=true) {
		if (!gene[i] && v) on++;
		else if (gene[i] && !v) on--;
		gene[i] = v;
	}
	void flip(int i) {
		gene[i] = !gene[i];
		if (gene[i]) on++;
		else on--;
	}
};

Chromosome::Chromosome() {
	fit = -1.0;
	on = 0;
	gene = vector<bool>(TR.N,false);
}

Chromosome::Chromosome(int _on) {
	on = _on;
	fit = -1.0;
	std::set<int> m;
	while (m.size()<_on)
		m.insert(rand() % TR.N);
	gene = vector<bool>(TR.N,false);
	for (std::set<int>::iterator its=m.begin(); its!=m.end(); ++its)
		gene[*its] = true;
}

Chromosome::Chromosome(vector<double> &vp) {
	on = 0;
	fit = -1.0;
	gene = vector<bool>(TR.N,false);
	for (int i=0 ; i<TR.N ; i++)
	if (drand() < vp[i]) {
		gene[i] = true;
		on++;
	}
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

void calc_fit_chromosome(Chromosome &c) {
	c.calc_fitness();
}
