#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<cstring>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

int main(int argc, char* argv[]) {

	string line;
	getline(cin,line);
	double time,red,tr,ts,
		_time,_red,_tr,_ts;
	for (int i=0 ; i<50 ; i++) {
		cin >> _time >> _red >> _tr >> _ts;
		time += _time;
		red += _red;
		tr += _tr;
		ts += _ts;
	}
	printf("%.2f\t%.2f\t%.2f\t%.2f\n",
		time/50.0,red/50.0,tr/50.0,ts/50.0);

	return 0;
}
