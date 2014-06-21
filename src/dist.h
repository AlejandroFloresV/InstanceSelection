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
	for (int i=0 ; i<K ; i++)
		d += (a[i]-b[i])*(a[i]-b[i]);
	return d;
}

double distBetween (int from, int to, bool tr) {
	if (tr) return DistTable[from][to];
	else return dist(TR[from],TS[to]);
}

double distSqrt (int from, int to, bool tr) {
	return sqrt(distBetween(from,to,tr));
}

void CalcDist () {
	DistTable = new double*[TR.N];
	for (int i=0 ; i<TR.N ; i++)
		DistTable[i] = new double[TR.N];
	for (int i=0 ; i<TR.N ; i++) {
		DistTable[i][i] = 0.0;
		for (int j=i+1 ; j<TR.N ; j++)
			DistTable[i][j] = DistTable[j][i] = dist(TR[i],TR[j]);
	}
}
