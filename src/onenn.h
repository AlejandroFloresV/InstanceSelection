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

class OneNN {
	ANNpointArray set;
	vector<int> index;
	ANNkd_tree* kdTree;
	ANNidxArray nnIdx;
	ANNdistArray dists;
	bool classify(int,bool);
	double error(bool);

	public:
	OneNN() {
		nnIdx = new ANNidx[1];
		dists = new ANNdist[1];
	}
	void CalcDist();
	void useJust(Chromosome);
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
			DistTable[i][j] = DistTable[j][i] = sqrt(annDist(NumClass,TR[i],TR[j]));
	}
}

void OneNN::useJust (Chromosome _sol) {
	index = _sol.get();
	set = new ANNpoint[index.size()];
	for (int i=index.size()-1 ; i>=0 ; i--)
		set[i] = TR[index[i]];
	kdTree = new ANNkd_tree(set,index.size(),NumClass);
}

bool OneNN::classify (int ind, bool tr) {
	kdTree->annkSearch((tr ? TR : TS)[ind],1,nnIdx,dists);
	return (tr ? TR : TS).Class[ind] == TR.Class[index[nnIdx[0]]];
}

double OneNN::error (bool tr) {
	if (index.size()==0) return 1.0;
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
	return errorTR()*ALPHA + ((double)index.size()/TR.N)*(1.0-ALPHA);
}

vector<double> OneNN::EnemyDistance(bool norm=true) {
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
	OneNN* mynn = new OneNN();
	mynn->useJust(*this);
	fit = mynn->fitnessAR();
}
