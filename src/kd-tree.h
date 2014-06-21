#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<climits>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

struct kd_order {
	public:
	int k;
	kd_order(int _k) : k(_k) {}
	bool operator() (int a, int b) {
		return (TR[a][k] < TR[b][k]);
	}
};

class kd_node {
	public:
	int p, l, r;
	kd_node() {}
};

class kd_tree {
	int next, *points, sp, nn;
	double radius;
	bool tr;
	kd_node *tree;
	int construct(int,int,int);
	void search_rec(int,int);
	void update_radius(int);

	public:
	kd_tree() {}
	kd_tree(Chromosome&);
	int search(int,bool);

};

kd_tree::kd_tree (Chromosome &p) {
	next = 0;
	points = new int[p.size()];
	tree = new kd_node[p.size()];
	for (int i=0, j=0 ; i<TR.N ; i++)
		if (p[i]) points[j++] = i;
	construct(0,0,p.size());
	delete points;
}

int kd_tree::construct (int attr, int l, int r) {
	if (r-l<1) return -1;
	kd_order ko(attr);
	sort(points + l, points + r, ko);
	int ind = next++, mean = (l+r)/2;
	for (; mean>=l ; mean--)
		if (mean==l || TR[points[mean-1]][attr] < TR[points[mean]][attr])
			break;
	tree[ind].p = points[mean];
	attr = (attr+1) % K;
	tree[ind].l = construct(attr,l,mean);
	tree[ind].r = construct(attr,mean+1,r);
	return ind;
}

int kd_tree::search (int _sp, bool _tr=true) {
	sp = _sp;
	tr = _tr;
	radius = numeric_limits<double>::max();
	search_rec(0,0);
	return nn;
}

void kd_tree::update_radius (int ind) {
	double d = distSqrt(ind,sp,tr);
	if (d < radius) {
		nn = ind;
		radius = d;
	}
}

void kd_tree::search_rec (int ind, int attr) {
	kd_node &n = ref(tree[ind]);
	if (n.l==-1 && n.r==-1) {
		// LEAF
		update_radius(n.p);
		return;
	}
	int new_attr = (attr+1) % K;
	if (n.l==-1 || (n.r!=-1 && TR[n.p][attr] <= (tr ? TR : TS)[sp][attr])) {
		// GO RIGHT
		search_rec(n.r,new_attr);
		if (n.l!=-1 && radius > ((tr ? TR : TS)[sp][attr] - TR[n.p][attr]))
			search_rec(n.l,new_attr);
	} else {
		// GO LEFT
		search_rec(n.l,new_attr);
		if (n.r!=-1 && radius > (TR[n.p][attr] - (tr ? TR : TS)[sp][attr]))
			search_rec(n.r,new_attr);
	}
	// CHECK CURRENT
	update_radius(n.p);
}
