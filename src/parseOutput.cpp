#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

int main() {

	char line[100];
	int min = 1;
	double red, trE, teE, seg;
	for (int i=0 ; i<10 ; i++) {
		for (int j=0 ; j<5 ; j++) {
			scanf("%lf\n",&red);
			scanf("%lf\n",&trE);
			scanf("%lf\n",&teE);
			scanf("\n");
			scanf("%s%dm%lfs\n",line,&min,&seg);
			scanf("%[^\n]\n",line);
			scanf("%[^\n]\n",line);

			printf("%lf\t%lf\t%lf\t%lf\n",((double)min*60.0)+seg,red,trE,teE);
		}
	}
	
	return 0;
}
