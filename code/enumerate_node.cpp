#include <iostream>
#include <omp.h>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;
const int maxn = 1000000;
int n; 				 // total number of vertex
int m;               // total number of edges
int num_thread = 4;
vector<int> G[maxn];
unordered_map<int, bool> has_edge[maxn];

void read_graph(const char* filename) {

	FILE* fp = fopen(filename, "r");
	int a, b;
 	fscanf(fp, "%d%d", &n, &m);

	while(fscanf(fp, "%d%d", &a, &b)==2) {
		G[a].push_back(b);
		G[b].push_back(a);
		has_edge[a][b] = 1;
		has_edge[b][a] = 1;
	}
}


bool check_edge(int a, int b) {
	if(has_edge[a].find(b) != has_edge[a].end()) {
		return true;
	}
	return false;
}

int base() {
	int cnt[] = {0, 0, 0, 0};

	omp_set_num_threads(num_thread);
	#pragma omp parallel for 
	for(int v=0; v<n; v++) {
		int tid = omp_get_thread_num();
		for(const auto& u: G[v]) if(u > v){
			for(const auto& w: G[u]) if(w > v and w > u) {
				if(check_edge(v, w) and check_edge(u, w)) {
					cnt[tid]++;
				}
			}
		}
	}
	return cnt[0] + cnt[1] + cnt[2] + cnt[3];
}

int main(int argc, char* argv[])
{
	read_graph(argv[1]);
	cout << "n = " << n << " m = " << m << endl;
	if(argc > 2) num_thread = atoi(argv[2]);
	cout << "Thrad_num = " << num_thread << endl;

	int ans = base();
	printf("Triangle count: %d\n", ans);
	return 0;
}

