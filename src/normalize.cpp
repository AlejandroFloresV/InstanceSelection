#include<iostream>
#include<cstdio>
#include<string>
#include<utility>
#include<algorithm>
#include<vector>
#include<map>

using namespace std;

#define MAXINST 1000000
#define MAXATTR 100

char   rowInd[MAXATTR+1];
double A[MAXINST * MAXATTR];
char   C[MAXINST], c;
int    countClass[1000];
double maxA[MAXATTR];
double minA[MAXATTR];
int    foldA[MAXINST];
int    temp[MAXINST];
map<string,char> mClass;
int N, K, ind, rows;
int count10fcv[10];

void addClass(string str) {
	if (mClass.find(str) == mClass.end()) {
		mClass[str] = c;
		C[N] = c++;
		countClass[C[N]] = 1;
	} else {
		C[N] = mClass[str];
		countClass[C[N]]++;
	}
}

int main (int argc, char *argv[]) {
	if (argc!=3) {
		printf("ERROR: Two arguments needed: <src_file> <dst_file>.\n");
		exit(1);
	}

	FILE *src, *dst;
	src = fopen(argv[1],"r");
	dst = fopen(argv[2],"w");
	if (src==NULL || dst==NULL) {
		printf("ERROR: opening file.\n");
		exit(1);
	}

	ind = K = N = c = 0;
	int offset = 0, rScan;
	char buff[10000],sep;

	rows = 0;
	for (int i=0 ; true ; i++) {
		rows++;
		fscanf(src,"%c%c",rowInd+i,&sep);
		if (rowInd[i]=='.') K++;
		if (sep=='\n') break;
	}

	for (int i=0 ; i<10 ; i++)
		count10fcv[i] = 0;

	while (!feof(src)) {
		for (int i=0,k=0 ; i<rows ; i++) {
			if (rowInd[i]=='c') {
				rScan = fscanf(src,"%[^,\n]%c",buff,&sep);
				if (rScan!=2) { N--; break; }
				addClass(string(buff));
			} else if (rowInd[i]=='.') {
				rScan = fscanf(src,"%lf%c",A+ind,&sep);
				if (rScan!=2) { N--; break; }
				if (ind<K) {
					maxA[k] = minA[k] = A[ind];
				} else {
					minA[k] = min(minA[k],A[ind]);
					maxA[k] = max(maxA[k],A[ind]);
				}
				ind++;
				k++;
			} else {
				rScan = fscanf(src,"%[^,\n]%c",buff,&sep);
				if (rScan!=2) { N--; break; }
			}
		}
		N++;
	}
	
	fclose(src);

	fprintf(dst,"%d %d\n",N,K);

	int seed = time(NULL);
	srand(seed);

	for (int i=0 ; i<c ; i++) {
		for (int n=0,j=0 ; n<N ; n++)
			if (C[n]==i)
				temp[j++] = n;
		for (int j=countClass[i]-1 ; j>0 ; j--) {
			int k = rand()%(j+1), t;
			t = temp[k];
			temp[k] = temp[j];
			temp[j] = t;
		}
		int each  = countClass[i] / 10,
			extra = countClass[i] % 10;
		int exInd[10];
		for (int j=0 ; j<10 ; j++) exInd[j] = 0;
		for (int j=0 ; j<extra ; j++) exInd[j] = 1;
		for (int j=9 ; j>0 ; j--) {
			int k = rand()%(j+1), t;
			t = exInd[k];
			exInd[k] = exInd[j];
			exInd[j] = t;
		}
		for (int j=0, f=0; j<countClass[i] ; f++) {
			count10fcv[f] += each+exInd[f];
			for (int e=0 ; e<each+exInd[f] ; e++, j++)
				foldA[temp[j]] = f;
		}
	}

	fprintf(dst,"%d",count10fcv[0]);
	for (int i=1 ; i<10 ; i++)
		fprintf(dst," %d",count10fcv[i]);
	fprintf(dst,"\n");

	ind = 0;
	for (int n=0 ; n<N ; n++) {
		fprintf(dst,"%d ",foldA[n]);
		for (int k=0 ; k<K ; k++,ind++)
			fprintf(dst,"%lf ", (maxA[k]==minA[k]
				? 0.0 : (A[ind]-minA[k])/(maxA[k]-minA[k])));
		fprintf(dst,"%d\n",1+(int)C[n]);
	}

	fclose(dst);

	return 0;
}
