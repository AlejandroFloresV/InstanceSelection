#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>
#include<map>

using namespace std;

// Some functions
#define b2i(a) (a ? 1 : 0)
#define b2d(a) (a ? 1.0 : 0.0)

double drand() {
	return (double)rand()/RAND_MAX;
}

void FatalError(string str) {
	printf("FATAL ERROR: %s\n",str.c_str());
	exit(0);
}

// Common Variables
int NumClass;
