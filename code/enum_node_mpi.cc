#include <iostream>
#include <omp.h>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <mpi.h>
#include <cassert>
#include <mpi.h>

using namespace std;
const int maxn = 1000000;
int n; 				 // total number of vertex
int m;               // total number of edges
int world_rank;
int world_size;
vector<int> G[maxn];
unordered_map<int, bool> has_edge[maxn];

void read_graph(const char* filename) {

	FILE* fp = fopen(filename, "r");
	int a, b;
 	assert(fscanf(fp, "%d%d", &n, &m));

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
  
	omp_set_num_threads(4);
	#pragma omp parallel for 
	for(int v=world_rank; v<n; v += world_size) {
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
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Status status;  
    
    read_graph(argv[1]);
	  cout << "n = " << n << " m = " << m << endl;
  	int ans = base();
    if(!world_rank){
        int tmp;
        for(int i = 1 ; i < world_size ; i++){
            MPI_Recv(&tmp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            ans += tmp; 
        }
  	    printf("Triangle count: %d\n", ans);
    }
    else{
        MPI_Send(&ans, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
  	return 0;
}