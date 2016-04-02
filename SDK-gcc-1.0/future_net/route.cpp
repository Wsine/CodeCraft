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

class Group {
public:
	Array points;
	Array route;
	double adapt;
	double p_live;
	bool visited[MAX_MATRIX_LENGTH];
	int max_depth;
/* Function */
	void initGroup(const Array& vec);
	void calFitness();
	void insertValue();
	void dfs(int from, int to, int depth);
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

void Group::initGroup(const Array& vec) {

}

void Group::calFitness() {

}

void Group::insertValue() {

}

void Group::dfs(int from, int to, int depth) {

}

void Nature::select() {

}

void Nature::cross() {

}

void Nature::hetero() {

}

bool Nature::cmp(const Group& a, const Group& b) {

}

