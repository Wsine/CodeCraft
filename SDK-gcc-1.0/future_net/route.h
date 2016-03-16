#ifndef __ROUTE_H__
#define __ROUTE_H__

void search_route(char *graph[5000], int edge_num, char *condition);
void read_map(char *topo[5000], int edge_num);
void read_aim(char *demand);
void dfs(int cur, int sum);
bool isConsist();
void record_path();
void print_map();
void print_aim();

#endif
