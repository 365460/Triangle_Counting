#include <iostream>
#include <assert.h>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;
const int maxn = 1000000;
const int K = 20;    // log2(maxn)
int n; 				 // total number of vertex
int m;               // total number of edges
vector<int> G[maxn];
vector<int> G2[maxn][K];
unordered_map<int, unordered_map<int, bool>> has_edge;

void read_graph(const char* filename) {

	FILE* fp = fopen(filename, "r");
	int a, b;
 	fscanf(fp, "%d%d", &n, &m);

	while(fscanf(fp, "%d%d", &a, &b)==2) {
		assert(a!=b);
		G[a].push_back(b);
		G[b].push_back(a);
		has_edge[a][b] = 1;
		has_edge[b][a] = 1;
	}
}

int DC(int from, int to) {
	int mid = (from+to)>>1;
	if(from + 2 >= to) {
		if(has_edge[from][to] and has_edge[from+1][to] 
				and has_edge[from][from+1]) 
			return 1;
		else return 0;
	}

	int a1 = DC(from, mid);
	int a2 = DC(mid+1, to);

	return a1 + a2;
}

void divide_edge() {
	for(int i=0; i<n; i++) {
		for(const auto &x: G[i]) {
			if(i > x) {

			} else {

			}
		}
	}
}

int main(int argc, char* argv[])
{
	read_graph(argv[1]);
	divide_edge();
	int ans = DC(1, n);
	
	cout << "n = " << n << " m = " << m << endl;
	printf("Triangle count: %d\n", ans);
	return 0;
}

