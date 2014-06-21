#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>
#include<cmath>

using namespace std;

class OneNN {
	kd_tree *t;
	Chromosome &sol, _init;
	//vector<int> subSet;
	int wrong(bool);
	bool classify(int,bool);

	public:
	OneNN () : sol(ref(_init)) {}
	void useJust(Chromosome&);
	bool classifyTR(int);
	bool classifyTS(int);
	double errorTR();
	double errorTS();
	int wrongTR();
	int wrongTS();
	double fitnessAR();
} NN;

void OneNN::useJust (Chromosome &_sol) {
	sol = _sol;
	t = new kd_tree(sol);
	//for (int i=0 ; i<TR.N ; i++)
	//	if (sol[i])
	//		subSet.push_back(i);
}

bool OneNN::classify (int ind, bool tr) {
	/*double d = (double)(K + 3), nd;
	char clss = 0;
	for (int i=0 ; i<subSet.size() ; i++) {
		if (ind==subSet[i])
			return true;
		else {
			nd = distSqrt(subSet[i],ind,tr);
			if (nd < d) {
				d = nd;
				clss = TR.Class[subSet[i]];
			}
		}
	}
	return (tr ? TR : TS).Class[ind] == clss;*/
	return (tr ? TR : TS).Class[ind] == TR.Class[t->search(ind,tr)];
}

int OneNN::wrong (bool tr) {
	if (sol.size()==0) return 1.0;
	int wrong = 0, tam = (tr ? TR.N : TS.N);
	for (int i=0 ; i<tam ; i++)
		if ((tr ? !sol[i] : true) && !classify(i,tr))
			wrong++;
	return wrong;
}

bool OneNN::classifyTR (int ind) {
	return (sol[ind] || classify(ind,true));
}
double OneNN::errorTR () {
	return (double)wrong(true)/TR.N;
}
int OneNN::wrongTR () {
	return wrong(true);
}

bool OneNN::classifyTS (int ind) {
	return classify(ind,false);
}
double OneNN::errorTS () {
	return (double)wrong(false)/TS.N;
}
int OneNN::wrongTS () {
	return wrong(false);
}

double OneNN::fitnessAR () {
	return errorTR()*ALPHA + ((double)sol.size()/TR.N)*(1.0-ALPHA);
}

vector<double> EnemyDistance(bool norm=true) {
	vector<double> CE(TR.N);
	double minD=-1.0,maxD=-1.0,currD;
	for (int i=0 ; i<TR.N ; i++) {
		CE[i] = -1.0;
		for (int j=0 ; j<TR.N ; j++) {
			if (TR.Class[i] != TR.Class[j]) {
				currD = DistTable[i][j];
				if (CE[i]<0.0 || currD<CE[i])
					CE[i] = currD;
			}
		}
		if (minD<0.0 || maxD<0.0)
			minD = maxD = CE[i];
		else {
			minD = min(minD,CE[i]);
			maxD = max(maxD,CE[i]);
		}
	}
	if (norm)
	for (int i=0 ; i<TR.N ; i++)
		CE[i] = (CE[i]-minD)/(maxD-minD);
	return CE;
}

vector<pair<double,int> > SortedEnemyDistance(bool norm=true) {
	vector<double> ed = EnemyDistance(norm);
	vector<pair<double,int> > cp(TR.N);
	for (int i=0 ; i<TR.N ; i++)
		cp[i] = make_pair(ed[i],i);
	sort(cp.begin(),cp.end());
	return cp;
}

/* +-------------------------------+ */
/* | Chromosome Fitness Definition | */
/* +-------------------------------+ */

double Chromosome::fitness() const {
	if (fit < 0.0) {
		Chromosome t = *this;
		NN.useJust(ref(t));
		fit = NN.fitnessAR();
	}
	return fit;
}

void Chromosome::calc_fitness() {
	OneNN mynn;
	mynn.useJust(*this);
	fit = mynn.fitnessAR();
}
