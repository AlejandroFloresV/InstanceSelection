#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

#define MAXINST 100000
#define MAXATTR 100

double setAttr[MAXINST * MAXATTR];
char   setClass[MAXINST];
int    originalIndex[MAXINST];

class InstanceSet {
	public:
	int N,begin;
	char *Class;
	int *Index;
	InstanceSet(){}
	InstanceSet(int n, int b) : N(n) {
		begin = b*NumClass;
		Class = setClass+b;
		Index = originalIndex+b;
	}
	double* operator[](const int&);
} TR,TS;

double* InstanceSet::operator[] (const int& ind) {
	return setAttr+begin+NumClass*ind;
}

void LoadData (string path, int testIndex) {

	if (testIndex<0 || 9<testIndex) {
		testIndex = 0;
		printf("ERROR: index for out of bounds. 0 selected.");
	}

	FILE *src;
	src = fopen(path.c_str(),"r");
	if (src==NULL)
		FatalError("Error opening file");
	
	int N,temp,trInd=0,tsInd=0;
	fscanf(src,"%d%d",&N,&NumClass);

	TR = InstanceSet(N,0);
	TS = InstanceSet(N,N-(N/10));

	double *attr;
	char *clss;
	for (int n=0 ; n<N ; n++) {
		fscanf(src,"%d",&temp);
		if (temp == testIndex) {
			clss = TS.Class + tsInd;
			TS.Index[tsInd] = n;
			attr = TS[tsInd++];
		} else {
			clss = TR.Class + trInd;
			TR.Index[trInd] = n;
			attr = TR[trInd++];
		}
		for (int k=0 ; k<NumClass ; k++)
			fscanf(src,"%lf",attr+k);
		fscanf(src,"%d",&temp);
		*clss = (char)temp;
	}

	TR.N = trInd;
	TS.N = tsInd;

	fclose(src);
}
