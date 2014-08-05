#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<cmath>

using namespace std;

#define TOURNAMENT_K 3
#define Offspring pair<Chromosome,Chromosome>
#define sortPopulation(p) sort(p.begin(),p.end())

// +----------------+
// | Some Functions |
// +----------------+

Population initPopFrom(Chromosome from) {
	Population pop(POP_SIZE);
	pop[0] = from;
	for (int i=1 ; i<POP_SIZE ; i++) {
		pop[i] = from;
		for (int j=0 ; j<TR.N ; j++)
			if (drand() < 0.35) pop[i].flip(j);
	}
	return pop;
}

int Hamming(Chromosome &a, Chromosome &b) {
	int hd = 0;
	for (int i=0 ; i<TR.N ; i++)
		if (a[i] != b[i])
			hd++;
	return hd;
}

// +----------------------+
// | Selection Algorithms |
// +----------------------+

int TournamentSel() {
	int indSel = POP_SIZE;
	for (int i=0 ; i<TOURNAMENT_K ; i++)
		indSel = min(indSel,rand() % POP_SIZE);
	return indSel;
}

Chromosome TournamentSel(Population *pop) {
	return (*pop)[TournamentSel()];
}

// +----------------------+
// | Crossover Algorithms |
// +----------------------+

Offspring CrossoverOnePoint(Chromosome &a, Chromosome &b) {
	int point = rand() % TR.N;
	Chromosome A, B;
	for (int i=0 ; i<TR.N ; i++) {
		A.set(i,i<point ? a[i] : b[i]);
		B.set(i,i<point ? b[i] : a[i]);
	}
	return make_pair(A,B);
}

Offspring CrossoverHUX(Chromosome &a, Chromosome &b) {
	int del = Hamming(a,b)/2;
	Chromosome A, B;
	bool bA, bB;
	for (int i=0 ; i<TR.N ; i++) {
		bA = a[i];
		bB = b[i];
		if (del>0 && bA != bB && drand()<0.5) {
			bA = !bA;
			bB = !bB;
			del--;
		}
		A.set(i,bA);
		B.set(i,bB);
	}
	return make_pair(A,B);
}

// +--------------------+
// | GENETIC ALGORITHMS |
// +--------------------+

Chromosome GGA() {

	Chromosome best, cA, cB;
	Population pop = initPopulation(),
		newPop(POP_SIZE);
	
	sortPopulation(pop);
	best = pop[0];

	for (int i=0 ; i<MAX_ITER ; i++) {
		vector<thread> t;
		for (int j=0 ; j<POP_SIZE ; j+=2) {
			cA = TournamentSel(&pop);
			cB = TournamentSel(&pop);
			if (drand() < CROSS_PROB) {
				Offspring crossAB = CrossoverOnePoint(ref(cA),ref(cB));
				cA = crossAB.first;
				cB = crossAB.second;
			}
			cA.mutate();
			newPop[j] = cA;
			t.push_back(thread(calc_fit_chromosome,ref(newPop[j])));
			if (j+1<POP_SIZE) {
				cB.mutate();
				newPop[j+1] = cB;
				t.push_back(thread(calc_fit_chromosome,ref(newPop[j+1])));
			}
		}

		for (int i=0 ; i<t.size() ; i++)
			t[i].join();
		pop = newPop;
		sortPopulation(pop);
		if (pop[0] < best) {
			best = pop[0];
			LAST_BEST_ITER = i+1;
		}
	}

	return best;
}

Chromosome SGA() {

	Chromosome best, cA, cB;
	Population pop = initPopulation(),
		temp = Population(4);
	int iA, iB;
	
	sortPopulation(pop);
	best = pop[0];

	for (int i=0 ; i<MAX_ITER ; i++) {
		iA = TournamentSel();
		iB = TournamentSel();
		temp[0] = pop[iA];
		temp[1] = pop[iB];
		if (drand() < CROSS_PROB) {
			Offspring crossAB = CrossoverOnePoint(ref(temp[0]),ref(temp[1]));
			temp[2] = crossAB.first;
			temp[3] = crossAB.second;
		} else {
			temp[2] = pop[iA];
			temp[3] = pop[iB];
		}
		temp[2].mutate();
		temp[3].mutate();
		sortPopulation(temp);
		pop[iA] = temp[0];
		pop[iB] = temp[1];
		sortPopulation(pop);
		if (pop[0] < best) {
			best = pop[0];
			LAST_BEST_ITER = i+1;
		}
	}

	return best;
}

Chromosome CHC() {

	Chromosome best;
	Population pop = initPopulation();
	int threshold = TR.N/4,
		iA, iB;

	sortPopulation(pop);
	best = pop[0];

	for (int i=0 ; i<MAX_ITER ; i++) {
		vector<thread> t;
		for (int j=0 ; j<POP_SIZE ; j+=2) {
			iA = rand() % POP_SIZE;
			iB = rand() % POP_SIZE;
			if (Hamming(ref(pop[iA]),ref(pop[iB])) > threshold) {
				Offspring crossAB = CrossoverHUX(ref(pop[iA]),ref(pop[iB]));
				pop.push_back(crossAB.first);
				t.push_back(thread(calc_fit_chromosome,ref(pop[pop.size()-1])));
				pop.push_back(crossAB.second);
				t.push_back(thread(calc_fit_chromosome,ref(pop[pop.size()-1])));
			}
		}

		for (int i=0 ; i<t.size() ; i++)
			t[i].join();

		if (pop.size()==POP_SIZE) {
			if (threshold==0) {
				pop = initPopFrom(best);
				threshold = TR.N/4;
			} else threshold--;
		} else {
			sortPopulation(pop);
			pop.resize(POP_SIZE);
			if (pop[0] < best) {
				best = pop[0];
				LAST_BEST_ITER = i+1;
			}
		}
	}

	return best;
}
