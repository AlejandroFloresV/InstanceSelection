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
	ANNpointArray Point;
	char* Class;
	int *Index;

	InstanceSet(){}
	InstanceSet(int n, int a) : N(n) {
		Point = annAllocPts(n,a);
		Class = new char[n];
		Index = new int[n];
	}
	ANNpoint operator[](const int& ind) {
		return Point[ind];
	}

} TR,TS;

void LoadData (string path, int testIndex) {

	FILE *src;
	src = fopen(path.c_str(),"r");
	if (src==NULL)
		FatalError("Error opening file");
	
	int N,temp,trInd,tsInd;
	fscanf(src,"%d%d",&N,&NumClass);

	trInd = tsInd = 0;
	for (int i=0 ; i<10 ; i++) {
		fscanf(src,"%d",&temp);
		if (i == testIndex)
			tsInd += temp;
		else trInd += temp;
	}

	TR = InstanceSet(trInd,NumClass);
	TS = InstanceSet(tsInd,NumClass);

	ANNpoint point;
	char *clss;
	trInd = tsInd = 0;
	for (int n=0 ; n<N ; n++) {
		fscanf(src,"%d",&temp);
		if (temp == testIndex) {
			TS.Index[tsInd] = n;
			clss = TS.Class + tsInd;
			point = TS[tsInd++];
		} else {
			TR.Index[trInd] = n;
			clss = TR.Class + trInd;
			point = TR[trInd++];
		}
		for (int k=0 ; k<NumClass ; k++)
			fscanf(src,"%lf",&point[k]);
		fscanf(src,"%d",&temp);
		*clss = (char)temp;
	}

	fclose(src);
}
