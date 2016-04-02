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
#define SELECT_DIVIDE 4
#define ITERATIONS 100



class Group {
public:
	Array points; //size(demands) + 2
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
	Nature();
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
int source, destination;
vector<int> v_demand;
int best_weight;

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand) {
    //unsigned short result[] = {2, 6, 3};//示例中的一个解

    startGene();
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

void read_demand(char *readline) {
	char *split = ",";
	char line[strlen(readline)];
	strcpy(line, readline);

	char* p = strtok(line, split);
	source = atoi(p);

	p = strtok(NULL, split);
	destination = atoi(p);;

	split = "|";
	while(true)
	{
		p = strtok(NULL, split);
		if(p == NULL || *p == '\n')
			break;
		v_demand.push_back(atoi(p));
	}
}

void record_path(const Array& path) {
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
	int map1[v_demand.size() + 2],map2[v_demand.size() + 2];

	for(int i = 0;i < crossNum.size() ;i += 2)
	{
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
		for(int j = p2 + 1;j < points.size() - 1;j++)
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
		
		for(int j = p2 + 1;j < points.size() - 1;j++)
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
		
		for(int j = p2 + 1;j < points.size() - 1;j++)
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

		swap(group[pos][p1], group[pos][p2]);
	}
}

bool Nature::cmp(const Group& a, const Group& b) {
	return a.adapt < b.adapt;
}


void startGene()
{
	Nature* nature = new Nature();
	nature->calFitness();

	for(int iter = 0;iter < ITERATIONS;i++){
		nature->select();
		nature->cross();
		nature->hetero();
		nature->calFitness();
	}

	int bestGroupIndex = 0;
	for(int i = 0;i < GROUP_NUM;i++)
		if(group[i].adapt < group[bestGroupIndex].adapt)
			bestGroupIndex = i;

	//NA
	if(nature->group[bestGroupIndex].route.empty()){
		delete nature;
		return ;
	}

	List& bestRoute = nature->group[bestGroupIndex].route;
	Array edge;
	int sum_cost = 0;
	printf("Route: ");
	List::iterator it = bestRoute.begin();
	for(;it + 1 != bestRoute.end();it++){
		sum_cost += matrix[*it][*(it + 1)].weight;
		edge.push_back(matrix[*it][*(it + 1)].index);
		printf("%d ",*it);
	}
	printf("%d \n",*bestRoute.rbegin()); //last element

	record_path(edge);

	delete nature;
}
