#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<cmath>

using namespace std;

Chromosome FarEnemyVoronoi() {

	vector<pair<double,int> > cp = SortedEnemyDistance(false);
	for (int i=0 ; i<TR.N ; i++)
		cp[i].first = sqrt(cp[i].first);

	vector<int> sel;
	int exclude = TR.N * EXCLUDE;
	for (int i=TR.N-1 ; i>=exclude ; i--) {
		bool add = true;
		for (int j=0 ; add && j<sel.size() ; j++)
			if (sqrt(DistTable[cp[i].second][cp[sel[j]].second])
					< cp[sel[j]].first + cp[i].first)
				add = false;
		if (add) sel.push_back(i);
	}

	Chromosome sol;
	for (int i=sel.size()-1 ; i>=0 ; i--)
		sol.set(cp[sel[i]].second);

	return sol;
}

Chromosome CNN() {
	Chromosome sol;
	sol.set(rand() % TR.N);
	NN.useJust(sol);

	while (true) {
		for (int i=0 ; i<TR.N ; i++)
		if (!NN.classifyTR(i)) {
			sol.set(i);
			NN.useJust(sol);
		}
		if (NN.wrongTR()==0)
			break;
	}
	return sol;
}
