#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <algorithm>
#include <sstream>
#include <limits.h>
#include <string.h>
#include <vector>
#include <iostream>
using namespace std;

#define MAX_MATRIX_LENGTH 600

struct Map {
	int index;
	int weight;
	Map() : index(-1), weight(-1) {}
	Map(int _index, int _weight) : index(_index), weight(_weight) {}
};

Map matrix[MAX_MATRIX_LENGTH][MAX_MATRIX_LENGTH];

int matrixLength;
bool visited[MAX_MATRIX_LENGTH];
vector<int> v_point;
vector<int> v_path;
vector<int> v_finalPath;
int source, destination;
vector<int> v_demand;
int best_weight;

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand) {
    read_map(topo, edge_num);
    print_map();
    read_aim(demand);
    print_aim();
    memset(visited, false, sizeof(visited));
    v_point.clear();
    v_path.clear();
    best_weight = INT_MAX;
    visited[source] = true;
    v_point.push_back(source);
    dfs(source, 0);
    record_path();
}

void read_map(char *topo[5000], int edge_num) {
	int index, from, to, weight;
	matrixLength = -1;
	for (int i = 0; i < edge_num; i++) {
		sscanf(topo[i], "%d,%d,%d,%d", &index, &from, &to, &weight);
		matrix[from][to] = Map(index, weight);
		if (from > matrixLength) matrixLength = from;
		if (to > matrixLength) matrixLength = to;
	}
	matrixLength++;
}

void read_aim(char *demand) {
	string s(demand);
	istringstream ss(s);
	char c;
	ss >> source >> c >> destination;
	int cond;
	while (ss >> c >> cond) {
		v_demand.push_back(cond);
	}
	sort(v_demand.begin(), v_demand.end());
}

void dfs(int cur, int sum) {
	if (cur == destination) {
		if (v_point.size() >= v_demand.size() + 2 && isConsist()) {
			if (sum < best_weight) {
				best_weight = sum;
				v_finalPath = v_path;
			}
		}
		return;
	}
	for (int i = 0; i < matrixLength; i++) {
		if (!visited[i] && matrix[cur][i].weight != -1) {
			visited[i] = true;
			v_point.push_back(i);
			v_path.push_back(matrix[cur][i].index);
			dfs(i, sum + matrix[cur][i].weight);
			v_path.pop_back();
			v_point.pop_back();
			visited[i] = false;
		}
	}
}

bool isConsist() {
	vector<int> v(v_point);
	sort(v.begin(), v.end());
	return (std::includes(v.begin(), v.end(), v_demand.begin(), v_demand.end()));
}

void record_path() {
	int size = v_finalPath.size();
	for (int i = 0; i < size; i++) {
		record_result(v_finalPath[i]);
	}
	printf("record finish...\n");
}

void print_map() {
	printf("The Map is :\n");
	printf("Map size is %d\n", matrixLength);
	for (int i = 0; i < matrixLength; i++) {
		for (int j = 0; j < matrixLength; j++) {
			printf("(%02d, %02d) ", matrix[i][j].index, matrix[i][j].weight);
		}
		printf("\n");
	}
	printf("\n");
}

void print_aim() {
	printf("The Demand is :\n");
	printf("source %d -> destination %d\n", source, destination);
	printf("aim is : ");
	int size = v_demand.size();
	for (int i = 0; i < size; i++) {
		printf("%d ", v_demand[i]);
	}
	printf("\n");
}
