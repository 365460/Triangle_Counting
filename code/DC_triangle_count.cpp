#include <iostream>
#include <assert.h>
#include <unordered_map>
#include <vector>
#include <thread>
#include <cmath>

using namespace std;
const int maxn = 100000 + 10;
const int K = 20;    // log2(maxn)
int num_thread = 4;
int n; 				 // total number of vertex
int m;               // total number of edges
vector<int> G[maxn];
vector<thread> threads;
vector<int*> ans;
unordered_map<int, vector<int>> work[maxn*K];
unordered_map<int, bool> has_edge[maxn];

// assign a < b
void divide_edge(int l, int r, int a, int b, int id) {
	// l r a b
	int mid = (l+r)>>1;
	// l a mid b r
	if(a <= mid and mid < b) {
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

bool check_edge(int a, int b) {
	if(has_edge[a].find(b) != has_edge[a].end()) {
		return true;
	}
	return false;
}

void thread_work(const int from, const int to, const int id, int* ans) {
	int tmp = 0;
	for(int a=from; a<=to; a++) if(work[id].find(a) != work[id].end()){
		for(const auto& b: work[id][a]) if(work[id].find(b) != work[id].end()){
			for(const auto& c: work[id][b]) {
				if(check_edge(a, c)) tmp++;
			}
		}
	}
	(*ans) = tmp/2;
}

void DC(int l, int r, int id) {
	if(l+1>=r) return;

	int mid = (l+r)>>1;

	int* a1 = new int();
	threads.emplace_back(thread_work, l, mid, id, a1);
	ans.push_back(a1);

	int* a2 = new int();
	threads.emplace_back(thread_work, mid+1, r, id, a2);
	ans.push_back(a2);

	DC(l, mid, id<<1);
	DC(mid+1, r, id<<1|1);
}

int main(int argc, char* argv[])
{
	read_graph(argv[1]);
	if(argc > 2) num_thread = atoi(argv[2]);
	
	cout << "n = " << n << " m = " << m << endl;
	cout << "Thread_num = " << num_thread << endl;

	DC(1, n, 1);
	cout << "DC ok" << endl;
	int Ans = 0;
	for(int i=0; i<threads.size(); i++) {
		threads[i].join();
		Ans += *(ans[i]);
	}
	printf("Triangle count: %d\n", Ans);
	return 0;
}

