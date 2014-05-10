#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>
#include<cmath>

using namespace std;

double** DistTable;

double dist(double *a, double *b) {
	double d = 0.0;
	for (int i=0 ; i<NumClass ; i++)
		d += (a[i]-b[i])*(a[i]-b[i]);
	return sqrt(d);
}

class OneNN {
	vector<int> subSet;
	bool classify(int,bool);
	double error(bool);

	public:
	OneNN() {};
	void CalcDist();
	void useJust(Chromosome);
	double distBetween(int,int,bool);
	bool classifyTR(int);
	bool classifyTS(int);
	double errorTR();
	double errorTS();
	double fitnessAR();
	vector<double> EnemyDistance(bool);
} NN;

void OneNN::CalcDist () {

	DistTable = new double*[TR.N];
	for (int i=0 ; i<TR.N ; i++)
		DistTable[i] = new double[TR.N];

	for (int i=0 ; i<TR.N ; i++) {
		DistTable[i][i] = 0.0;
		for (int j=i+1 ; j<TR.N ; j++)
			DistTable[i][j] = DistTable[j][i] = dist(TR[i],TR[j]);
	}
}

void OneNN::useJust (Chromosome _sol) {
	subSet = _sol.get();
}

bool OneNN::classify (int ind, bool tr) {
	double d = (double)(NumClass + 3), nd;
	char clss = 0;
	for (int i=0 ; i<subSet.size() ; i++) {
		if (ind==subSet[i])
			return true;
		else {
			nd = distBetween(subSet[i],ind,tr);
			if (nd < d) {
				d = nd;
				clss = TR.Class[subSet[i]];
			}
		}
	}
	return (tr ? TR : TS).Class[ind] == clss;
}

double OneNN::distBetween (int from, int to, bool tr) {
	if (tr) return DistTable[from][to];
	else return dist(TR[from],TS[to]);
}

double OneNN::error (bool tr) {
	int wrong = 0, tam = (tr ? TR.N : TS.N);
	for (int i=0 ; i<tam ; i++)
		if (!classify(i,tr)) wrong++;
	return (double)wrong/tam;
}

bool OneNN::classifyTR (int ind) {
	return classify(ind,true);
}

double OneNN::errorTR () {
	return error(true);
}

bool OneNN::classifyTS (int ind) {
	return classify(ind,false);
}

double OneNN::errorTS () {
	return error(false);
}

double OneNN::fitnessAR () {
	return errorTR()*ALPHA + ((double)subSet.size()/TR.N)*(1.0-ALPHA);
}

vector<double> OneNN::EnemyDistance(bool norm=true) {
	vector<double> CE(TR.N);
	double minD=-1.0,maxD=-1.0,currD;
	for (int i=0 ; i<TR.N ; i++) {
		CE[i] = -1.0;
		for (int j=0 ; j<TR.N ; j++) {
			if (TR.Class[i] != TR.Class[j]) {
				currD = distBetween(i,j,true);
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


/* +-------------------------------+ */
/* | Chromosome Fitness Definition | */
/* +-------------------------------+ */

double Chromosome::fitness() const {
	if (fit < 0.0) {
		NN.useJust(*this);
		fit = NN.fitnessAR();
	}
	return fit;
}

void Chromosome::calc_fitness() {
	OneNN mynn;
	mynn.useJust(*this);
	fit = mynn.fitnessAR();
}
