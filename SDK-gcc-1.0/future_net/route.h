#ifndef __ROUTE_H__
#define __ROUTE_H__

#define MAX_POINTS 600

void search_route(char *graph[5000], int edge_num, char *condition);
void read_topo(char *topo[], int edge_num);
void read_demand(char *demand); 
void BFS(int V);

struct Edge{
    int from;
    int to;
    int index;
    int weight;
//    Edge(){};
//    Edge(int _from, int _to, int _index, int _weight){
//        from = _from;
//        to = _to;
//        index = _index;
//        weight = _weight;
//    };
//    ~Edge(){};
};

#endif
