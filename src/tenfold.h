#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

class InstanceSet {

	public:
	int N;
	double* Attr;
	char* Class;
	int *Index;

	InstanceSet(){}
	InstanceSet(int n) : N(n) {
		Attr = new double[n*K];
		Class = new char[n];
		Index = new int[n];
	}
	double* operator[](const int& ind) {
		return &(this->Attr[K*ind]);
	}

} TR,TS;

void LoadData (string path, int testIndex) {

	FILE *src;
	src = fopen(path.c_str(),"r");
	if (src==NULL)
		FatalError("Error opening file");
	
	int N,temp,trInd,tsInd;
	fscanf(src,"%d%d",&N,&K);

	trInd = tsInd = 0;
	for (int i=0 ; i<10 ; i++) {
		fscanf(src,"%d",&temp);
		if (i == testIndex)
			tsInd += temp;
		else trInd += temp;
	}

	TR = InstanceSet(trInd);
	TS = InstanceSet(tsInd);

	double *attr;
	char *clss;
	trInd = tsInd = 0;
	for (int n=0 ; n<N ; n++) {
		fscanf(src,"%d",&temp);
		if (temp == testIndex) {
			TS.Index[tsInd] = n;
			clss = TS.Class + tsInd;
			attr = TS[tsInd++];
		} else {
			TR.Index[trInd] = n;
			clss = TR.Class + trInd;
			attr = TR[trInd++];
		}
		for (int k=0 ; k<K ; k++)
			fscanf(src,"%lf",&attr[k]);
		fscanf(src,"%d",&temp);
		*clss = (char)temp;
	}

	fclose(src);
}
