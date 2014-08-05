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
int K;
bool verbose = true;
unsigned int RUN_SEED;
int LAST_BEST_ITER = 0;

// GENERAL
double ALPHA = 0.5;
double COMP_ALPHA;
int MAX_ITER = 10000;
int POP_SIZE = 50;
double MUT_PROB = 0.001;
string INIT_TYPE = "Random";
double BIT_PROB = 0.05;

// PBIL
double MS = 0.01;
double LR = 0.2;
double NLR = 0.075;

// PSO
int PARTICLES = 15;
double Vmax = 0.05;
double C1 = 0.2;
double C2 = 0.2;
double INERTIA = 0.6;

// GENETIC
double CROSS_PROB = 1.0;

// NEHS
double EXCLUDE = 0.33333;
