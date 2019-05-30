#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include "queue.h"
#include "stack.h"

#define SIZE 200

typedef struct graph {
	int V;
	List *adjLists;
}*Graph;

Graph initGraph(int V);
Graph insertEdge(Graph g, int u, int v, char *name);
Graph deleteVertex(Graph g, int v);
int getInDegree(Graph g, int v);
void printGraph(Graph g);
void drawGraph(Graph g, char *name);
void bfs(Graph g, int start);
void dfs(Graph g, int start, int *visited);
int elemConexe(Graph g, int *visited);
Graph freeGraph(Graph g);

#endif