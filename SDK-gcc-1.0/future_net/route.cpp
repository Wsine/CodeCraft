#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <string.h>
#include <list>

std::list<Edge> adj_list[MAX_POINTS];
int required_V[MAX_POINTS] = {0};
int start_V = -1;
int end_V = -1;

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
    read_topo(topo, edge_num);
    read_demand(demand);



    unsigned short result[] = {2, 6, 3};//示例中的一个解

    for (int i = 0; i < 3; i++)
        record_result(result[i]);
}

void read_topo(char *topo[], int edge_num){
    for(int i = 0; i < edge_num; i++){
        Edge te;
        sscanf(topo[i], "%d,%d,%d,%d", 
           &te.index, &te.from, &te.to, &te.weight);
        adj_list[te.from].push_back(te);
        printf("read %d edges\n", i);
    }
}

void read_demand(char *demand){
    char reqs[3000];
    sscanf(demand, "%d,%d,%s", &start_V, &end_V, reqs);

    char *pch;
    int temp;
    memset(required_V, 0, sizeof(int) * MAX_POINTS);
    pch = strtok(reqs, "|");
    while(pch != NULL){
        sscanf(pch, "%d", &temp);
        required_V[temp] = 1;
        pch = strtok(NULL, "|");
    }
}


