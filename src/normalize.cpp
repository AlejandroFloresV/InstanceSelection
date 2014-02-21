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

char rowInd[MAXATTR+1];
double A[MAXINST * MAXATTR];
char   C[MAXINST], c;
double maxA[MAXATTR];
double minA[MAXATTR];
map<string,char> mClass;
int N, K, ind, rows;
int count10fcv[10];

void addClass(string str) {
	if (mClass.find(str) == mClass.end()) {
		mClass[str] = ++c;
		C[N] = c;
	} else {
		C[N] = mClass[str];
	}
}

int main (int argc, char *argv[]) {
	if (argc!=4) {
		printf("ERROR: Two arguments needed: <src_file> <dst_file> <# rows>.\n");
		exit(1);
	}

	FILE *src, *dst;
	src = fopen(argv[1],"r");
	dst = fopen(argv[2],"w");
	if (src==NULL || dst==NULL) {
		printf("ERROR: opening file.\n");
		exit(1);
	}

	rows = atoi(argv[3]);
	if (rows<2 || 100<rows) {
		printf("ERROR: wrong # of rows.\n");
		exit(1);
	}
	ind = K = N = c = 0;
	int offset = 0, rScan;
	char buff[10000],sep;

	for (int i=0 ; i<rows ; i++) {
		fscanf(src,"%c%c",rowInd+i,&sep);
		if (rowInd[i]=='.') K++;
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

	for (int n=0 ; n<N ; n++)
		count10fcv[rand() % 10]++;

	fprintf(dst,"%d",count10fcv[0]);
	for (int i=1 ; i<10 ; i++)
		fprintf(dst," %d",count10fcv[i]);
	fprintf(dst,"\n");

	srand(seed);
	ind = 0;
	for (int n=0 ; n<N ; n++) {
		fprintf(dst,"%d ",rand() % 10);
		for (int k=0 ; k<K ; k++,ind++)
			fprintf(dst,"%lf ", (maxA[k]==minA[k]
				? 0.0 : (A[ind]-minA[k])/(maxA[k]-minA[k])));
		fprintf(dst,"%d\n",(int)C[n]);
	}

	fclose(dst);

	return 0;
}
