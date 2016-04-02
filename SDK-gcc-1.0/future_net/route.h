#ifndef __ROUTE_H__
#define __ROUTE_H__

#define MAX_POINTS 600

void search_route(char *graph[5000], int edge_num, char *condition);
void read_topo(char *topo[], int edge_num);
void read_demand(char *demand); 
void BFS(int V);
void check_end_vertex();

struct Edge{
    int from;
    int to;
    int index;
    int weight;
};

#endif
