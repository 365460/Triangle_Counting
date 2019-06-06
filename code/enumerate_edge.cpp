#include <iostream>
#include <omp.h>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;
const int maxn = 10000000;
int n; 				 // total number of vertex
int m;               // total number of edges
int num_thread = 4;

vector<int> G[maxn];
vector<pair<int, int> > Edge;
unordered_map<int, bool> has_edge[maxn];

void read_graph(const char* filename) {

  	FILE* fp = fopen(filename, "r");
  	int a, b;
   	fscanf(fp, "%d%d", &n, &m);
    
  	while(fscanf(fp, "%d%d", &a, &b)==2) {
        if(a > b) swap(a, b);
    		Edge.push_back(make_pair(a, b));
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

int base_edge() {
    int cnt[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    clock_t clk_min[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    clock_t clk_max[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
  	omp_set_num_threads(num_thread);
  	#pragma omp parallel for schedule(dynamic)
    for(int i = 0 ; i < Edge.size(); i++){
        int tid = omp_get_thread_num();
        if(!clk_min[tid])  clk_min[tid] = clock();
          
        int a = Edge[i].first, b = Edge[i].second;
        for(int j = 0 ; j < G[a].size() ; j++){
            int w = G[a][j];
            if(b < w && check_edge(b, w))
                cnt[tid]++;
        }
        clk_max[tid] = clock();
    }
    for(int i = 0 ; i < num_thread ; i++)
      cout << clk_max[i] - clk_min[i] << " ";
    cout << endl; 
    
    int ans = 0;
    for(int i = 0 ; i < num_thread ; i++)
        ans += cnt[i];
  	return ans;
}

int main(int argc, char* argv[])
{
  	read_graph(argv[1]);
  	cout << "n = " << n << " m = " << m << endl;
    if(argc > 2) num_thread = atoi(argv[2]);
	  cout << "Thread_num = " << num_thread << endl;
  	int ans = base_edge();
  	printf("Triangle count: %d\n", ans);
  	return 0;
}