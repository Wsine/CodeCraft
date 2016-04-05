#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <algorithm>
#include <sstream>
#include <limits.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <list>
#include <map>
using namespace std;

#define DEBUG 0
#define MAX_MATRIX_LENGTH 600
#define INF 1e8
#define GROUP_NUM 500
#define CROSS_PROB 0.8
#define HETERO_PROB 0.6
#define SELECT_DIVIDE 4
#define DFS_PROB 0.8
#define ITERATIONS 500

typedef vector<int> Array;
typedef list<int> List;

class Group {
public:
	Array points;
	List route;
	int adapt;
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
	void dfsInit(Array& path);
	void dfs(int from, int to, int weight, int depth, Array& path);
	void print();
	void printGene();
};

class Nature{
public:
	Group group[GROUP_NUM];
/* Function */
	// Nature();
	void select();
	void cross();
	void hetero();
	void calAllFitness();
	void print();
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
int source, destination;
vector<int> v_demand;
List bestRouteInHistory;
int best_weight;

void record_path(Array&);
void print_route(List&);

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand) {
	read_map(topo, edge_num);
	#if DEBUG
	// print_map();
	#endif
	read_demand(demand);
	#if DEBUG
	// print_demand();
	#endif
	best_weight = INF;
	startGene();
}

void read_map(char *topo[5000], int edge_num) {
	int index, from, to, weight;
	matrixLength = -1;
	for (int i = 0; i < edge_num; i++) {
		sscanf(topo[i], "%d,%d,%d,%d", &index, &from, &to, &weight);

		if (matrix[from][to].weight == -1) {
			matrix[from][to] = Map(index, weight);
		} else if (weight < matrix[from][to].weight) {
			matrix[from][to] = Map(index, weight);
		}
		if (from > matrixLength) matrixLength = from;
		if (to > matrixLength) matrixLength = to;

		bool existed = false;
		for (unsigned int j = 0; j < table[from].size(); j++) {
			if (table[from][j] == to) {
				existed = true;
				break;
			}
		}
		if (!existed)	table[from].push_back(to);
	}
	matrixLength++;
}

void read_demand(char *readline) {
	char reqs[3000];
	sscanf(demand, "%d,%d,%s", &source, &destination, reqs);

	char *pch;
	int temp;
	pch = strtok(reqs, "|");
	while(pch != NULL){
		sscanf(pch, "%d", &temp);
		v_demand.push_back(temp);
		pch = strtok(NULL, "|");
	}
}

void record_path(Array& path) {
	Array::iterator it = path.begin();
	for (; it != path.end(); it++) {
		record_result(*it);
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

void print_demand() {
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
	this->adapt = -1;
	this->p_live = -1.0;
	initGroup();
}

void Group::initGroup() {
	// memset(this->visited, false, sizeof(this->visited));
	this->points.clear();
	this->points.push_back(source);
	// this->visited[source] = true;
	vector<int>::iterator it = v_demand.begin();
	for (; it != v_demand.end(); it++) {
		this->points.push_back(*it);
		// this->visited[*it] = true;
	}
	random_shuffle(this->points.begin() + 1, this->points.end());
	this->points.push_back(destination);
	// this->visited[destination] = true;
}

void Group::calFitness() {
	this->insertValue();
	#if DEBUG
	// printf("insert successfully\n");
	#endif
	bool pass = true;
	int weight = 0;
	List::iterator itList = this->route.begin();
	List::iterator itListNext = this->route.begin();
	itListNext++;
	for (; itListNext != this->route.end(); itList++, itListNext++) {
		if (matrix[*itList][*itListNext].weight != -1)
			weight += (matrix[*itList][*itListNext].weight);
		else {
			pass = false;
			break;
		}
	}
	if (!pass) {
		this->route.clear();
	} else {
		this->adapt = weight;
		if (weight < best_weight) {
			best_weight = weight;
			bestRouteInHistory = this->route;
		}
	}
}

void Group::insertValue() {
	memset(this->visited, false, sizeof(this->visited));
	this->route.clear();
	Array::iterator it = this->points.begin();
	for (; it != this->points.end(); it++) {
		this->route.push_back(*it);
		this->visited[*it] = true;
	}

	Array dfs_path;
	List::iterator itList = this->route.begin();
	List::iterator itListNext = itList;
	itListNext++;
	for (; itListNext != this->route.end(); itList++, itListNext++) {
		if (matrix[*itList][*itListNext].weight == -1) {
			dfsInit(dfs_path);
			dfs_path.push_back(*itList);
			dfs(*itList, *itListNext, 0, 0, dfs_path);

			if (dfs_part_route.size() >= 3) {
				it = dfs_part_route.begin() + 1;
				for (; it + 1 != dfs_part_route.end(); it++) {
					this->visited[*it] = true;
					this->route.insert(itListNext, *it);
					itList++;
				}
			}
		} else {
			srand((unsigned)time(NULL));
			double cross_p = (rand() % 100) * 1.0 / 100;
			if (cross_p < DFS_PROB) {
				dfsInit(dfs_path);
				this->dfs_weight = matrix[*itList][*itListNext].weight;
				dfs_path.push_back(*itList);
				dfs(*itList, *itListNext, 0, 0, dfs_path);

				if (dfs_part_route.size() >= 3) {
					it = dfs_part_route.begin() + 1;
					for (; it + 1 != dfs_part_route.end(); it++) {
						this->visited[*it] = true;
						this->route.insert(itListNext, *it);
						itList++;
					}
				}
			}
		}
	}
}

void Group::dfsInit(Array& path) {
	memcpy(this->dfs_visited, this->visited, sizeof(this->dfs_visited));
	path.clear();
	this->dfs_part_route.clear();
	if (matrixLength < 50)
		this->dfs_max_depth = 5;
	else
		this->dfs_max_depth = matrixLength / 10;
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
		if (!this->dfs_visited[*it] || *it == to) {
			this->dfs_visited[*it] = true;
			path.push_back(*it);
			dfs(*it, to, weight + matrix[from][*it].weight, depth + 1, path);
			path.pop_back();
			this->dfs_visited[*it] = false;
		}
	}
}

void Group::print() {
	if (!this->route.empty()) {
		List::iterator itList = this->route.begin();
		for (; itList != this->route.end(); itList++) {
			printf("%d ", *itList);
		}
		printf("\n");
	} else {
		printf("route empty\n");
	}
}

void Group::printGene() {
	Array::iterator it = this->points.begin();
	for (; it != this->points.end(); it++) {
		printf("%d ", *it);
	}
	printf("\n");
}

void Nature::print() {
	printf("\n");
	for (int i = 0; i < GROUP_NUM; i++) {
		printf("Group %d : ", i);
		group[i].printGene();
	}
}

void Nature::calAllFitness()
{
	for(int i = 0;i < GROUP_NUM;i++){
		group[i].calFitness();
	}
}
void Nature::select() {
	sort(group, group + GROUP_NUM, cmp);
	int bais = GROUP_NUM/SELECT_DIVIDE;
	/*Select the top 25% and worest 25%*/

	for(int i  = 0;i < bais;i++){
		group[i + bais] = group[i];
	}
	
	for(int i  = bais * (SELECT_DIVIDE - 1);i < GROUP_NUM;i++){
		group[i - bais] = group[i];
	}
}

void Nature::cross() {
	Array crossNum;
	srand((unsigned)time(NULL));

	for(int i = 0;i < GROUP_NUM;i++){
		double cross_p = (rand() % 100) * 1.0 / 100; //计算交配概率
		if(cross_p < CROSS_PROB)
			crossNum.push_back(i);
	}

	int p1,p2 ; //交换基因断点 
	map<int, int> map1,map2;

	for(unsigned int i = 0;i < crossNum.size() ;i += 2)
	{
		if (i + 1 >= crossNum.size())	break;
		int n1 = crossNum[i];
		int n2 = crossNum[i + 1];
		
		p1 = rand() % (v_demand.size()) + 1;   //get a number from 1 to size - 2, totally (size - 2)
		p2 = rand() % (v_demand.size()) + 1;

		if(p1 > p2)swap(p1 , p2);

		//断点之间的基因产生映射
		for(int j = p1;j <= p2;j++)
		{
			map1[group[n1].points[j]]=group[n2].points[j];
			map2[group[n2].points[j]]=group[n1].points[j];
		}

		//断点两边的基因互换
		//Pay attention to the begin point and the dest point
		for(int j = 1;j < p1;j++)
		{
			swap(group[n1].points[j] , group[n2].points[j]);
		}
		for(unsigned int j = p2 + 1;j < group[n1].points.size() - 1;j++)
		{
			swap(group[n1].points[j] , group[n2].points[j]);
		}

		//处理产生的冲突基因
		for(int j = 1;j < p1;j++)
			for(int k = p1;k <= p2;k++)
			{
				if(group[n1].points[j] == group[n1].points[k])
				{
					group[n1].points[j]=map1[group[n1].points[j]];
					k = p1 - 1;       //从头开始找 
				}
			}
		
		for(unsigned int j = p2 + 1;j < group[n1].points.size() - 1;j++)
			for(int k = p1;k <= p2;k++)
			{
				if(group[n1].points[j] == group[n1].points[k])
				{
					group[n1].points[j]=map1[group[n1].points[j]];
					k = p1 - 1;
				}
			}
		
		for(int j = 1;j < p1;j++)
			for(int k = p1;k <= p2;k++)
			{
				if(group[n2].points[j] == group[n2].points[k])
				{
					group[n2].points[j]=map2[group[n2].points[j]];
					k = p1 - 1;
				}
			}
		
		for(unsigned int j = p2 + 1;j < group[n2].points.size() - 1;j++)
			for(int k = p1;k <= p2;k++)
			{
				if(group[n2].points[j] == group[n2].points[k])
				{
					group[n2].points[j]=map2[group[n2].points[j]];
					k = p1 - 1;
				}
			}
	}


}

void Nature::hetero() {
	Array heteroNum;//需要变异的编号 
	srand((unsigned)time(NULL));
	
	for(int i = 0;i < GROUP_NUM;i++)
	{
		//计算交配概率
		double hetero_p = (rand() % 100)*1.0 / 100;
		if(hetero_p < HETERO_PROB)
			heteroNum.push_back(i);				
	}
	
	//随机选两个相邻位置基因交换 
	for(Array::iterator it = heteroNum.begin();it != heteroNum.end();it++)
	{
		int pos = *it;
		int p1 = rand() % (v_demand.size()) + 1;   //get a number from 1 to size - 2, totally (size - 2)
		int p2 = rand() % (v_demand.size()) + 1;

		swap(group[pos].points[p1], group[pos].points[p2]);
	}
}

bool Nature::cmp(const Group& a, const Group& b) {
	return a.adapt < b.adapt;
}


void startGene()
{
	Nature* nature = new Nature();
	nature->calAllFitness();


	for(int iter = 0;iter < ITERATIONS;iter++){
		nature->select();
#if DEBUG
		printf("select successfully\n");
#endif
		nature->cross();
#if DEBUG
		printf("cross successfully\n");
#endif
		nature->hetero();
#if DEBUG
		printf("hetero successfully\n");
#endif
		nature->calAllFitness();
#if DEBUG
		nature->print();
#endif
	}

	int bestGroupIndex = 0;
	for(int i = 0;i < GROUP_NUM;i++)
		if(nature->group[i].adapt < nature->group[bestGroupIndex].adapt)
			bestGroupIndex = i;

	//NA
	if(bestRouteInHistory.empty()){
		delete nature;
		return ;
	}

	Array edge;
	int sum_cost = 0;
	printf("Route: ");
	List::iterator it_next,it_cur;
	it_next = it_cur = bestRouteInHistory.begin();
	it_next++;

	for(;it_next != bestRouteInHistory.end();it_cur++,it_next++){
		sum_cost += matrix[*it_cur][*it_next].weight;
		edge.push_back(matrix[*it_cur][*it_next].index);
		printf("%d ",*it_cur);
	}
	printf("%d \n",*it_cur); //last element

	record_path(edge);

	delete nature;
}

void print_route(List& arr) {
	printf("Route: ");
	List::iterator it_cur;
	it_cur = arr.begin();

	for(;it_cur != arr.end();it_cur++){
		printf("%d ",*it_cur);
	}
	printf("\n");
}
