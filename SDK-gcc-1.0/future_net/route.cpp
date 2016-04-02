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
#define INF 1e8
#define GROUP_NUM 100
#define CROSS_PROB 0.8
#define HETERO_PROB 0.6

typedef vector<int> Array;
typedef list<int> List;

class Group {
public:
	Array points;
	List route;
	double adapt;
	double p_live;
	bool visited[MAX_MATRIX_LENGTH];
	bool dfs_visited[MAX_MATRIX_LENGTH];
	Array dfs_part_route;
	int dfs_weight;
	int dfs_max_depth;
/* Function */
	Group();
	void initGroup();
	void calFitness();
	void insertValue();
	void dfsInit();
	void dfs(int from, int to, int weight, int depth);
};

class Nature{
public:
	Group group[GROUP_NUM];
/* Function */
	void select();
	void cross();
	void hetero();
	static bool cmp(const Group& a, const Group& b);
};

struct Map {
	int index;
	int weight;
	Map() : index(-1), weight(-1) {}
	Map(int _index, int _weight) : index(_index), weight(_weight) {}
};

Map matrix[MAX_MATRIX_LENGTH][MAX_MATRIX_LENGTH];
vector<int> table[MAX_MATRIX_LENGTH];

int matrixLength;
vector<int> v_finalPath;
int source, destination;
vector<int> v_demand;
int best_weight;

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand) {
    unsigned short result[] = {2, 6, 3};//示例中的一个解

    for (int i = 0; i < 3; i++)
        record_result(result[i]);
}

void read_map(char *topo[5000], int edge_num) {
	int index, from, to, weight;
	matrixLength = -1;
	for (int i = 0; i < edge_num; i++) {
		sscanf(topo[i], "%d,%d,%d,%d", &index, &from, &to, &weight);

		if (matrix[from][to].weight != -1 && weight < matrix[from][to].weight) {
			matrix[from][to] = Map(index, weight);
		} else if (matrix[from][to].weight == -1) {
			matrix[from][to] = Map(index, weight);
		}
		if (from > matrixLength) matrixLength = from;
		if (to > matrixLength) matrixLength = to;

		bool existed = false;
		for (int j = 0; j < table[from].size(); j++) {
			if (table[from][j] == to) {
				existed = true;
				break;
			}
		}
		if (!existed)	table[from].push_back(to);
	}
	matrixLength++;
}

void read_demand(char *demand) {
	// TODO 
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

Group::Group() {
	this->route.clear();
	this->adapt = -1.0;
	this->p_live = -1.0;
	initGroup();
}

void Group::initGroup() {
	memset(this->visited, false, sizeof(this->visited));
	this->points.clear();
	this->points.push_back(source);
	this->visited[source] = true;
	vector<int>::iterator it = v_demand.begin();
	for (; it != v_demand.end(); it++) {
		this->points.push_back(*it);
		this->visited[*it] = true;
	}
	random_shuffle(this->points.begin() + 1, this->points.end());
	this->points.push_back(destination);
	this->visited[destination] = true;
}

void Group::calFitness() {

}

void Group::insertValue() {
	
}

void Group::dfsInit() {
	memcpy(this->dfs_visited, this->visited, sizeof(this->dfs_visited));

	this->dfs_part_route.clear();
	if (matrixLength < 20)
		this->dfs_max_depth = 2;
	else
		this->dfs_max_depth = dfs_max_depth / 10;
	this->dfs_weight = INF;
}

void Group::dfs(int from, int to, int weight, int depth, Array& path) {
	if (from == to) {
		if (weight < dfs_weight) {
			dfs_weight = weight;
			this->dfs_part_route = path;
		}
		return;
	}
	if (depth >= dfs_max_depth) return;
	vector<int>::iterator it = table[from].begin();
	for (; it != table[from].end(); it++) {
		if (!this->dfs_visited[*it]) {
			this->dfs_visited[*it] = true;
			path.push_back(*it);
			dfs(*it, to, weight + matrix[from][*it], depth + 1, path);
			path.pop_back();
			this->dfs_visited[*it] = false;
		}
	}
}

void Nature::select() {

}

void Nature::cross() {

}

void Nature::hetero() {

}

bool Nature::cmp(const Group& a, const Group& b) {

}

