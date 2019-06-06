#include <iostream>
#include <omp.h>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <mpi.h>
#include <cassert>

using namespace std;
const int maxn = 1000000;
int n; 				 // total number of vertex
int m;               // total number of edges
int world_size;
int world_rank;
int num_thread = 4;

vector<int> G[maxn];
vector<pair<int, int> > Edge;
unordered_map<int, bool> has_edge[maxn];

void read_graph(const char* filename) {

  	FILE* fp = fopen(filename, "r");
  	int a, b;
   	assert(fscanf(fp, "%d%d", &n, &m));
    
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
    for(int i = world_rank ; i < Edge.size(); i += world_size){
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
    for(int i = 0 ; i < num_thread ; i++)
        cout << clk_min[i] << " ";
    cout << endl;  
    for(int i = 0 ; i < num_thread ; i++)
        cout << clk_max[i] << " ";
    cout << endl; 
    int ans = 0;
    for(int i = 0 ; i < num_thread ; i++)
        ans += cnt[i];
  	return ans;
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Status status;
    
  	read_graph(argv[1]);
  	cout << "n = " << n << " m = " << m << endl;
  	int ans = base_edge();
    if(!world_rank){
        int tmp;
        for(int i = 1 ; i < world_size ; i++){
            MPI_Recv(&tmp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            ans += tmp;
            printf("i = %d %d\n", i, tmp);  
        }
  	    printf("Triangle count: %d\n", ans);
    }
    else{
        MPI_Send(&ans, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
  	return 0;
}