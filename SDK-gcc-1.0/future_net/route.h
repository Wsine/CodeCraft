#ifndef __ROUTE_H__
#define __ROUTE_H__

typedef vector<int> Array;

void search_route(char *graph[5000], int edge_num, char *condition);
void read_map(char *topo[5000], int edge_num);
void read_demand(char *demand);
void record_path(const Array&);
void print_map();
void print_aim();
void startGene();

#endif
