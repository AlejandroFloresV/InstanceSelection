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
	exit(1);
}

// Common Variables
int NumClass;
bool verbose = true;
unsigned int RUN_SEED;

// GENERAL
int MAX_ITER = 10000;
int POP_SIZE = 50;
double MUT_PROB = 0.001;
string INIT_TYPE = "Random";

// PBIL
double MS = 0.01;
double LR = 0.2;
double NLR = 0.075;

// PSO
int PARTICLES = 15;
double C1 = 3.0;
double C2 = 1.0;

// GENETIC
double CROSS_PROB = 1.0;

// FarEnemyVoronoi
double EXCLUDE = 0.33333;
