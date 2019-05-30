#include "graph.h"

Graph initGraph(int V)
{
	Graph graph = malloc(sizeof(struct graph));
	graph->V = V;
	graph->adjLists = calloc(V,sizeof(struct list));
	return graph;
}

Graph insertEdge(Graph g, int u, int v,  char *name)
{
	g->adjLists[u] = addLast(g->adjLists[u], v, name);
	g->adjLists[v] = addLast(g->adjLists[v], u, name);
	return g;
}

Graph deleteVertex(Graph g, int v)
{	
	int i;
	for(i = 0; i < g->V; i++)
		if(i != v)
		{
			g->adjLists[i] = deleteItem(g->adjLists[i], v);
		}
	g->adjLists[v] = freeList(g->adjLists[v]);
	return g;
}

void printGraph(Graph g)
{
	int i;
	for(i = 0; i < g->V; i++)
	{
		if(g->adjLists[i] != NULL)
			printList(g->adjLists[i]);
	}
}

Graph freeGraph(Graph g)
{
	int i = 0;
	for(i = 0; i < g->V; i++)
	{
		freeList(g->adjLists[i]);
	}
	free(g->adjLists);
	free(g);
	
	return NULL;
}

void bfs(Graph g, int start)
{
	int visited[g->V];
	int i;
	for(i = 0; i < g->V; i++)
		visited[i] = 0;

	Queue q = NULL;
	visited[start] = 1;
	q = enqueue(q, start);

	while(!(isEmptyQueue(q)))
	{
		start = first(q);
		printf("%d\n", start);
		q = dequeue(q);
		List temp = g->adjLists[start];
		while(temp != NULL)
		{
			if(!visited[temp->id])
			{
				visited[temp->id] = 1;
				q = enqueue(q, temp->id);
			}
			temp = temp->next;
		}
	}
}

int elemConexe(Graph g, int *visited)
{
	int i, nrElemConexe = 0;
	for(i = 0; i < g->V; i++)
	{
		if(visited[i] == 0)
		{
			dfs(g, i, visited);
			nrElemConexe++;
		}
	}	
	return nrElemConexe;
}

void dfs(Graph g, int start, int *visited)
{
	visited[start] = 1;
	List temp = g->adjLists[start];
	while(temp != NULL)
	{
		if(!visited[temp->id])
			dfs(g, temp->id, visited);
		temp = temp->next;
	}
}

void drawGraph(Graph g, char *name)
{
	int i;
	FILE *stream;
	char *buffer;
	List tmp;
	if (g == NULL || name == NULL)
		return;
	stream = fopen(name, "w");
	fprintf(stream, "graph G {\n");
	fprintf(stream, "    node [fontname=\"Arial\", shape=circle, style=filled, fillcolor=yellow];\n");
	for (i = 0; i < g->V; i++) {
		tmp = g->adjLists[i];
		while (tmp != NULL) {
			if (tmp->id > i)
				fprintf(stream, "    %d -- %d;\n", i, tmp->id);
			tmp = tmp->next;
		}
	}
	fprintf(stream, "}\n");
	fclose(stream);
	buffer = (char*) malloc(SIZE*sizeof(char));
	sprintf(buffer, "dot %s | neato -n -Tpng -o graph.png", name);
	system(buffer);
	free(buffer);
}