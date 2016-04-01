#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <string.h>
#include <list>
#include <queue>
#include <set>

std::list<Edge> adj_list[MAX_POINTS];
std::list<Edge> r_adj_list[MAX_POINTS];
Edge edges[MAX_POINTS * 8];
std::queue<int> search_queue;
std::set<std::list<int> > reach[MAX_POINTS];
std::set<int> edge_visited;

int required_V[MAX_POINTS] = {0};
int start_V = -1;
int end_V = -1;

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
    read_topo(topo, edge_num);
    read_demand(demand);

    search_queue.push(start_V);
    while(!search_queue.empty()){
        BFS(search_queue.front());
        search_queue.pop();
    }


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
        r_adj_list[te.to].push_back(te);
        edges[te.index] = te;
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

void BFS(int V){
    for(std::list<Edge>::iterator it_edge = adj_list[V].begin();
            it_edge != adj_list[V].end(); it_edge ++){
        // record the number of routes of next vertex
        unsigned int next_v_routes = reach[it_edge->to].size();
        for(std::set<std::list<int> >::iterator it_route = reach[V].begin();
                it_route != reach[V].end(); it_route ++){
            // new route circle check
            int has_circle = 0;
            if(it_edge->to == start_V){
                has_circle = 1;
            }
            for(std::list<int>::const_iterator it_v = it_route->begin();
                    it_v != it_route->end(); it_v ++){
                if(it_edge->to == edges[*it_v].to){
                   has_circle = 1; 
                }
            }
            // add new route to next vertex
            if(!has_circle){
                std::list<int> new_route = *it_route;
                new_route.push_back(it_edge->to);
                reach[it_edge->to].insert(new_route);
            }
            else
                continue;
        }
        // update search queue
        if(reach[it_edge->to].size() > next_v_routes)
            search_queue.push(it_edge->to);
    }
    return;
}
