#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<cmath>

using namespace std;

Chromosome FarEnemyVoronoi() {

	vector<double> ED = NN.EnemyDistance(false);
	vector<pair<double,int> > cp(TR.N);

	for (int i=0 ; i<TR.N ; i++)
		cp[i] = make_pair(ED[i],i);
	sort(cp.begin(),cp.end());
	vector<int> sel;

	int exclude = TR.N * EXCLUDE;
	for (int i=TR.N-1 ; i>=exclude ; i--) {
		bool add = true;
		for (int j=0 ; add && j<sel.size() ; j++)
			if (DistTable[cp[i].second][cp[sel[j]].second]
					< cp[sel[j]].first + cp[i].first)
				add = false;
		if (add) sel.push_back(i);
	}

	vector<int> chromSel(sel.size());
	for (int i=0 ; i<sel.size() ; i++)
		chromSel[i] = cp[sel[i]].second;

	sort(chromSel.begin(),chromSel.end());

	return Chromosome(chromSel);
}
