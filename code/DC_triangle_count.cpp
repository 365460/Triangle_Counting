#include <iostream>
#include <assert.h>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;
const int maxn = 1000 + 10;
const int K = 20;    // log2(maxn)
int n; 				 // total number of vertex
int m;               // total number of edges
vector<int> G[maxn];
vector<int> work[maxn*K][maxn];
unordered_map<int, unordered_map<int, bool>> has_edge;

// assign a < b
void divide_edge(int l, int r, int a, int b, int id) {
	// l r a b
	int mid = (l+r)>>1;
	// l a mid b r
	if(a <= mid and mid <=b) {
		work[id][a].push_back(b);
		work[id][b].push_back(a);
	}
	// l mid a b r
	else if (mid < a) divide_edge(mid+1, r, a, b, id<<1|1);
	// l a b mid r
	else divide_edge(l, mid, a, b, id<<1);
}

void read_graph(const char* filename) {

	FILE* fp = fopen(filename, "r");
	int a, b;
 	fscanf(fp, "%d%d", &n, &m);

	while(fscanf(fp, "%d%d", &a, &b)==2) {
		a++, b++;
		assert(a!=b);
		G[a].push_back(b);
		G[b].push_back(a);
		if(a<b) divide_edge(1, n, a, b, 1);
		else divide_edge(1, n, b, a, 1);

		has_edge[a][b] = 1;
		has_edge[b][a] = 1;
	}
}

int DC(int l, int r, int id) {
	if(l==r) return 0;
	int ans = 0;
	int mid = (l+r)>>1;
	for(int i=0; i<mid; i++) {
		for(int j=0; j<work[id][i].size(); j++) {
			int to = work[id][i][j];
			for(int k=0; k<work[id][to].size(); k++) {
				int to2 = work[id][to][k];
				if(has_edge[to2][i]) ans++;
			}
		}
	}

	for(int i=mid; i<r; i++) {
		for(int j=0; j<work[id][i].size(); j++) {
			int to = work[id][i][j];
			for(int k=0; k<work[id][to].size(); k++) {
				int to2 = work[id][to][k];
				if(has_edge[to2][i]) ans++;
			}
		}
	}
	return ans + DC(l, mid, id<<1) + DC(mid+1, r, id<<1|1);
}

int main(int argc, char* argv[])
{
	read_graph(argv[1]);
	int ans = DC(1, n, 1);
	
	cout << "n = " << n << " m = " << m << endl;
	printf("Triangle count: %d\n", ans);
	return 0;
}

