#include <iostream>
#include <unordered_map>
#include <cmath>
#include <cstdlib>

using namespace std;

unordered_map<int, unordered_map<int, bool>> used;

bool check_edge(int a, int b) {
	if(a==b) return false;
	if(used.find(a) != used.end()) {
		if(used[a].find(b) != used[a].end()) { // edge exists
			return false;
		}
	}
	return true;
}

int main(int argc, char* argv[]) {
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	const char* file_out = argv[3];

	FILE* fp = fopen(file_out, "w");

	cout << "Write to " << file_out << endl;

	fprintf(fp, "%d %d\n", n, m);
	for(int i=0; i<m; i++) {
		int a, b;
		do {
			a = rand()%n;
			b = rand()%n;
		} while(check_edge(a, b) == false);
		used[a][b] = used[b][a] = 1;
		fprintf(fp, "%d %d\n", a, b);
	}
	return 0;
}
