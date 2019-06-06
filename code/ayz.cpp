#include <iostream>
#include <omp.h>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;
const int maxn = 1000000;
int n; 				 // total number of vertex
int m;         // total number of edges
int num_thread = 4;

vector<int> G[maxn];
vector<pair<int, int> > Edge, Edge_enum;
unordered_map<int, bool> has_edge[maxn], enum_node;

void read_graph(const char* filename) {

  	FILE* fp = fopen(filename, "r");
  	int a, b;
   	fscanf(fp, "%d%d", &n, &m);
    
  	while(fscanf(fp, "%d%d", &a, &b)==2) {
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

int base_node() {
  	int cnt[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    clock_t clk_min[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    clock_t clk_max[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  	omp_set_num_threads(num_thread);
  	#pragma omp parallel for schedule(dynamic)
  	for(int v=0; v<n; v++) {
        if(!enum_node[v]) continue;
    		int tid = omp_get_thread_num();
        if(!clk_min[tid])  clk_min[tid] = clock();
    		for(const auto& u: G[v]) if(enum_node[u] and u > v){
      			for(const auto& w: G[u]) if(enum_node[w] and w > v and w > u) {
      				if(check_edge(v, w) and check_edge(u, w)) {
      					cnt[tid]++;
      				}
      			}
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

int base_edge() {
  	double cnt[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    clock_t clk_min[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    clock_t clk_max[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
  	omp_set_num_threads(num_thread);
  	#pragma omp parallel for schedule(dynamic)
    for(int i = 0 ; i < Edge_enum.size(); i++){
        int tid = omp_get_thread_num();
        if(!clk_min[tid])  clk_min[tid] = clock();
        int a = Edge_enum[i].first, b = Edge_enum[i].second;
        if(enum_node[a]) swap(a, b);
        for(int j = 0 ; j < G[a].size() ; j++){
            int w = G[a][j];
            if(check_edge(b, w)){
                if(enum_node[b] && enum_node[w])
                    cnt[tid] += 1.5;
                else
                    cnt[tid]++;    
            }
        }
        clk_max[tid] = clock();
    }
    
  	for(int i = 0 ; i < num_thread ; i++)
      cout << clk_max[i] - clk_min[i] << " ";
    cout << endl; 
    
    double ans = 0;
    for(int i = 0 ; i < num_thread ; i++)
        ans += cnt[i];
  	return ans;
}

void devide(){
    int thr = sqrt(m);
    printf("thr = %d\n", thr);
    //omp_set_num_threads(4);
  	//#pragma omp parallel for
    for(int i = 0 ; i < n ; i++){
        if(G[i].size() > thr){
            //printf("%d %d\n", i, G[i].size());
            enum_node[i] = 1;
        }
    }
  	
    for(int i = 0 ; i < m ; i++){
        int a = Edge[i].first, b = Edge[i].second;
        if(!enum_node[a] || !enum_node[b])
            Edge_enum.push_back(make_pair(a, b));
    }
    printf("edge_enum size = %d\n", Edge_enum.size());

}

int main(int argc, char* argv[])
{
  	read_graph(argv[1]);
    if(argc > 2) num_thread = atoi(argv[2]);
	  cout << "Thread_num = " << num_thread << endl;
  	cout << "n = " << n << " m = " << m << endl;
    devide();
  	int ans = base_edge() / 3 + base_node(); 
  	printf("Triangle count: %d\n", ans);
  	return 0;
}