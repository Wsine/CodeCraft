#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <string.h>
#include <list>
#include <queue>
#include <set>
#include <vector>
#include <map>
#include <algorithm>

int weight_sum(std::list<int> route);

std::list<Edge> adj_list[MAX_POINTS];
std::list<Edge> r_adj_list[MAX_POINTS];
Edge edges[MAX_POINTS * 8];
std::queue<int> search_queue;
std::set<std::list<int> > reach[MAX_POINTS];
std::set<int> edge_visited;
std::vector<int> required_V;

int start_V = -1;
int end_V = -1;

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
    read_topo(topo, edge_num);
    read_demand(demand);

    std::list<int> l;
    reach[start_V].insert(l);

    search_queue.push(start_V);
    while(!search_queue.empty()){
        BFS(search_queue.front());
        search_queue.pop();
    }

    printf("finish BFS\n");

    printf("there are %d ways to reach end_V: \n", (int) reach[end_V].size());

    std::set<std::list<int> >::iterator it1;
    std::list<int>::const_iterator it2;
    for(it1 = reach[end_V].begin(); it1 != reach[end_V].end(); it1 ++){
        for(it2 = it1->begin(); it2 != it1->end(); it2 ++){
            printf("%d->", *it2);
        }
        printf("done!\n");
    }
    

    check_end_vertex();

/*
    unsigned short result[] = {2, 6, 3};//示例中的一个解

    for (int i = 0; i < 3; i++)
        record_result(result[i]);
*/
}

void read_topo(char *topo[], int edge_num){
    for(int i = 0; i < edge_num; i++){
        Edge te;
        sscanf(topo[i], "%d,%d,%d,%d", 
           &te.index, &te.from, &te.to, &te.weight);
        adj_list[te.from].push_back(te);
        r_adj_list[te.to].push_back(te);
        edges[te.index] = te;
    }
}

void read_demand(char *demand){
    char reqs[3000];
    sscanf(demand, "%d,%d,%s", &start_V, &end_V, reqs);

    char *pch;
    int temp;
    pch = strtok(reqs, "|");
    while(pch != NULL){
        sscanf(pch, "%d", &temp);
        required_V.push_back(temp);
        pch = strtok(NULL, "|");
    }
}

void BFS(int V){
    printf("now BFSing Vertex: %d\n", V);
    for(std::list<Edge>::iterator it_edge = adj_list[V].begin();
            it_edge != adj_list[V].end(); it_edge ++){
        // record the number of routes of next vertex
        unsigned int next_v_routes = reach[it_edge->to].size();
        printf("routes of vertex %d before insert: %d\n", it_edge->to, next_v_routes);
        for(std::set<std::list<int> >::iterator it_route = reach[V].begin();
                it_route != reach[V].end(); it_route ++){
            // new route circle check
            int has_circle = 0;
            if(it_edge->to == start_V){
                has_circle = 1;
            }
            for(std::list<int>::const_iterator it_index = it_route->begin();
                    it_index != it_route->end(); it_index ++){
                if(it_edge->to == edges[*it_index].to){
                   has_circle = 1;
                }
            }
            // add new route to next vertex
            if(!has_circle){
                std::list<int> new_route = *it_route;
                new_route.push_back(it_edge->index);
                reach[it_edge->to].insert(new_route);
            }
            else
                continue;
        }
        // update search queue
        printf("routes of vertex %d after insert: %d\n", it_edge->to, (int) reach[it_edge->to].size());
        if(reach[it_edge->to].size() > next_v_routes){
            search_queue.push(it_edge->to);
            printf("pushed vertex: %d into search_queue\n", it_edge->to);
        }
    }
    return;
}

void check_end_vertex(){
    std::map<std::list<int>, int > good_routes;

    // check satisfies required
    for(std::set<std::list<int> >::iterator it_route = reach[end_V].begin();
            it_route != reach[end_V].end(); it_route ++){
        int satisfy = 1;
        for(std::vector<int>::iterator it_int = required_V.begin();
                it_int != required_V.end(); it_int ++){
            std::list<int>::const_iterator it_index;
            it_index = find(it_route->begin(), it_route->end(), *it_int);
            for(it_index = it_route->begin(); it_index != it_route->end(); it_index ++){
                if(*it_int == edges[*it_index].to)
                    break;
            }
            if(it_index == it_route->end()){
                satisfy = 0;
                break;
            }
        }
        // put and calculate the weight_sum
        if(satisfy){
            printf("satisfies, weight_sum = %d\n", weight_sum(*it_route));
            good_routes.insert(std::pair<std::list<int>, int>
                    (*it_route, weight_sum(*it_route)));
        }
        else{
            printf("not satisfy\n");
        }
    }
    
    if(good_routes.empty())
        return;
    else{
        // printf("now printing out good_routes...\n");

        // select the shortest path
        std::map<std::list<int>, int>::iterator min_route = good_routes.begin();
        for(std::map<std::list<int>, int>::iterator it_route = good_routes.begin();
                it_route != good_routes.end(); it_route ++){
            if(it_route->second < min_route->second){
                min_route = it_route;
            }
        }
        // record the result
        for(std::list<int>::const_iterator it_index = min_route->first.begin();
                it_index != min_route->first.end(); it_index ++){
            record_result(*it_index);
        }
    }
}

int weight_sum(std::list<int> route){
    int sum = 0;
    for(std::list<int>::iterator it_index = route.begin();
            it_index != route.end(); it_index ++)
        sum += edges[*it_index].weight;
    return sum;
}
