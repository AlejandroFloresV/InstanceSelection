#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<cmath>

using namespace std;

#define TOURNAMENT_K 3
#define Population vector<Chromosome>
#define Offspring pair<Chromosome,Chromosome>
#define sortPopulation(p) sort(p.begin(),p.end())

// +----------------+
// | Some Functions |
// +----------------+

Population initPopRandom() {
	Population pop(POP_SIZE);
	for (int i=0 ; i<POP_SIZE ; i++)
		pop[i] = Chromosome((int)round(TR.N*(0.04+0.02*drand())));
	return pop;
}

Population initPopFrom(Chromosome from) {
	Population pop(POP_SIZE);
	pop[0] = from;
	for (int i=1 ; i<POP_SIZE ; i++) {
		from.iterator();
		vector<int> v;
		for (int j=0 ; j<TR.N ; j++) {
			bool iB = from.next();
			if (drand() < 0.35) iB = !iB;
			if (iB) v.push_back(i);
		}
		pop[i] = Chromosome(v);
	}
	return pop;
}

Population initPopEnemyProb(bool closest) {

	vector<double> Vp = NN.EnemyDistance();
	vector<pair<double,int> > cp(TR.N);

	for (int i=0 ; i<TR.N ; i++)
		cp[i] = make_pair(Vp[i],i);
	sort(cp.begin(),cp.end());
	Vp = vector<double>(TR.N,0.02);

	for (int i=(closest ? 0 : TR.N-1), j=TR.N/35 ;
		i>=0 && j>=0 ; (closest ? i++ : i--), j--)
		Vp[cp[i].second] = 0.9;

	Population pop(POP_SIZE);
	for (int i=0 ; i<POP_SIZE ; i++)
		pop[i] = Chromosome(Vp);
	return pop;
}

Population initPop() {
	Population p;
	if (INIT_TYPE == "Random")
		p = initPopRandom();
	else if (INIT_TYPE == "ClosestEnemy")
		p = initPopEnemyProb(true);
	else if (INIT_TYPE == "FarthestEnemy")
		p = initPopEnemyProb(false);
	else FatalError("Wrong type of population initialization.");
	return p;
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
	return make_pair(Chromosome(vA),Chromosome(vB));
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
	return make_pair(Chromosome(vA),Chromosome(vB));
}

// +--------------------+
// | GENETIC ALGORITHMS |
// +--------------------+

Chromosome GGA() {

	Chromosome best, cA, cB;
	Population pop = initPop(),
		newPop(POP_SIZE);
	
	sortPopulation(pop);
	best = pop[0];

	for (int i=0 ; i<MAX_ITER ; i++) {
		for (int j=0 ; j<POP_SIZE ; j+=2) {
			cA = TournamentSel(&pop);
			cB = TournamentSel(&pop);
			if (drand() < CROSS_PROB) {
				Offspring crossAB = CrossoverOnePoint(cA,cB);
				cA = crossAB.first;
				cB = crossAB.second;
			}
			cA.mutate();
			newPop[j] = cA;
			if (j+1<POP_SIZE) {
				cB.mutate();
				newPop[j+1] = cB;
			}
		}
		pop = newPop;
		sortPopulation(pop);
		if (pop[0] < best)
			best = pop[0];
	}

	return best;
}

Chromosome SGA() {

	Chromosome best, cA, cB;
	Population pop = initPop(),
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
			Offspring crossAB = CrossoverOnePoint(temp[0],temp[1]);
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
	}

	return best;
}

Chromosome CHC() {

	Chromosome best;
	Population pop = initPop();
	int threshold = TR.N/4,
		iA, iB;

	sortPopulation(pop);
	best = pop[0];

	for (int i=0 ; i<MAX_ITER ; i++) {
		for (int j=0 ; j<POP_SIZE ; j+=2) {
			iA = rand() % POP_SIZE;
			iB = rand() % POP_SIZE;
			if (Hamming(pop[iA],pop[iB]) > threshold) {
				Offspring crossAB = CrossoverHUX(pop[iA],pop[iB]);
				pop.push_back(crossAB.first);
				pop.push_back(crossAB.second);
			}
		}
		
		sortPopulation(pop);
		if (pop[0] < best)
			best = pop[0];

		if (pop.size()==POP_SIZE) {
			if (threshold==0) {
				pop = initPopFrom(best);
				threshold = TR.N/4;
			} else threshold--;
		}

		pop.resize(POP_SIZE);
	}

	return best;
}
