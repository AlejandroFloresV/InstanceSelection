#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<cmath>

using namespace std;

#define GENETIC_POP 50
#define TOURNAMENT_K 3
#define Population vector<Chromosome>
#define Offspring pair<Chromosome,Chromosome>
#define sortPopulation(p) sort(p.begin(),p.end())

// +----------------+
// | Some Functions |
// +----------------+

Population initPop() {
	Population pop(GENETIC_POP);
	for (int i=0 ; i<GENETIC_POP ; i++)
		pop[i] = Chromosome(TR.N,(int)round(TR.N*(0.04+0.02*drand())));
	return pop;
}

Population initPopFrom(Chromosome from) {
	Population pop(GENETIC_POP);
	pop[0] = from;
	for (int i=1 ; i<GENETIC_POP ; i++) {
		from.iterator();
		vector<int> v;
		for (int j=0 ; j<TR.N ; j++) {
			bool iB = from.next();
			if (drand() < 0.35) iB = !iB;
			if (iB) v.push_back(i);
		}
		pop[i] = Chromosome(TR.N,v);
	}
	return pop;
}

int Hamming(Chromosome a, Chromosome b) {
	a.iterator();
	b.iterator();
	int hd = 0;
	for (int i=0 ; i<TR.N ; i++)
		if (a.next()!=b.next())
			hd++;
	return hd;
}

// +----------------------+
// | Selection Algorithms |
// +----------------------+

Chromosome TournamentSel(Population *pop) {
	Population tourPop(TOURNAMENT_K);
	for (int i=0 ; i<TOURNAMENT_K ; i++)
		tourPop[i] = (*pop)[rand() % GENETIC_POP];
	sortPopulation(tourPop);
	return tourPop[0];
}

// +----------------------+
// | Crossover Algorithms |
// +----------------------+

Offspring CrossoverOnePoint(Chromosome a, Chromosome b) {
	int point = rand() % TR.N;
	vector<int> vA, vB;
	bool bA, bB;
	a.iterator();
	b.iterator();
	for (int i=0 ; i<TR.N ; i++) {
		bA = (i<point ? a.next() : b.next());
		bB = (i<point ? b.next() : a.next());
		if (bA) vA.push_back(i);
		if (bB) vB.push_back(i);
	}
	return make_pair(Chromosome(TR.N,vA),Chromosome(TR.N,vB));
}

Offspring CrossoverHUX(Chromosome a, Chromosome b) {
	int del = Hamming(a,b)/2;
	vector<int> vA, vB;
	bool bA, bB;
	a.iterator();
	b.iterator();
	for (int i=0 ; i<TR.N ; i++) {
		bA = a.next();
		bB = b.next();
		if (del>0 && bA != bB && drand()<0.5) {
			bA = !bA;
			bB = !bB;
			del--;
		}
		if (bA) vA.push_back(i);
		if (bB) vB.push_back(i);
	}
	return make_pair(Chromosome(TR.N,vA),Chromosome(TR.N,vB));
}

// +--------------------+
// | GENETIC ALGORITHMS |
// +--------------------+

#define GGA_MAXITER 10000
#define GGA_CROSS_PROB 0.6

Chromosome GGA() {

	Chromosome best, cA, cB;
	Population pop = initPop(),
		newPop(GENETIC_POP);
	
	sortPopulation(pop);
	best = pop[0];

	for (int i=0 ; i<GGA_MAXITER ; i++) {
		for (int j=0 ; j<GENETIC_POP ; j+=2) {
			cA = TournamentSel(&pop);
			cB = TournamentSel(&pop);
			Offspring crossAB = CrossoverOnePoint(cA,cB);
			if (drand() < GGA_CROSS_PROB) {
				cA = crossAB.first;
				cB = crossAB.second;
			}
			newPop[ j ] = cA;
			newPop[ j ].mutate();
			newPop[j+1] = cB;
			newPop[j+1].mutate();
		}
		pop = newPop;
		sortPopulation(pop);
		if (pop[0].fitness() < best.fitness())
			best = pop[0];
	}

	return best;
}

#define SGA_MAXITER 10000

Chromosome SGA() {

	Chromosome best, cA, cB;
	Population pop = initPop();
	
	sortPopulation(pop);
	best = pop[0];

	for (int i=0 ; i<SGA_MAXITER ; i++) {
		cA = TournamentSel(&pop);
		cB = TournamentSel(&pop);
		Offspring crossAB = CrossoverOnePoint(cA,cB);
		pop.push_back(crossAB.first);
		pop.push_back(crossAB.second);
		sortPopulation(pop);
		pop.pop_back();
		pop.pop_back();
		if (pop[0].fitness() < best.fitness())
			best = pop[0];
	}

	return best;
}

#define CHC_MAXITER 10000

Chromosome CHC() {

	Chromosome best;
	Population pop = initPop();
	int threshold = TR.N/4,
		iA, iB;

	sortPopulation(pop);
	best = pop[0];

	for (int i=0 ; i<CHC_MAXITER ; i++) {
		for (int j=0 ; j<GENETIC_POP ; j+=2) {
			iA = rand() % GENETIC_POP;
			iB = rand() % GENETIC_POP;
			if (Hamming(pop[iA],pop[iB]) > threshold) {
				Offspring crossAB = CrossoverHUX(pop[iA],pop[iB]);
				pop.push_back(crossAB.first);
				pop.push_back(crossAB.second);
			}
		}
		
		sortPopulation(pop);
		if (pop[0].fitness() < best.fitness())
			best = pop[0];

		if (pop.size()==GENETIC_POP) {
			if (threshold==0) {
				pop = initPopFrom(best);
				threshold = TR.N/4;
			} else threshold--;
		}

		pop.resize(GENETIC_POP);
	}

	return best;
}
