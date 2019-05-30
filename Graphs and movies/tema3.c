#include "graph.h"

Graph GraphBuilder(Graph graph, List *l, char* in,
				char* out, char*actor1, char* actor2)
{
	int id = 0, i, n, nr, cont = 0;
	FILE *fp = fopen(in,"r");
	size_t buffsize = 32;
	char *buff = malloc(buffsize*sizeof(char));
	getline(&buff, &buffsize, fp);
	char *movie = malloc(75*sizeof(char));
	char *actor = malloc(75*sizeof(char));
	*l = NULL;
	n = atoi(buff);
	for(i = 0; i < n; i++)
	{
		getline(&movie, &buffsize, fp);
		getline(&buff, &buffsize, fp);
		nr = atoi(buff);
		int j, id = 0;
		for(j = 0; j < nr; j++)
		{
			getline(&actor, &buffsize, fp);
			if(!*l)
			{
				*l = initList(cont, actor);
				cont++;
			}
			else
			{
				List tmp = *l;
				bool ok = false;
				while(tmp)
				{
					if(strcmp(actor, tmp->name) == 0)
					{
						ok = true;
						cont--;
						break;
					}
					tmp = tmp->next;
				}
				if(!ok)
					*l = addLast(*l, cont, actor);
				cont++;		
			}
		}
	}
	fclose(fp);
	graph = initGraph(cont);
	fp = fopen(in,"r");
	getline(&buff, &buffsize, fp);
	cont = 0;
	for(i = 0; i < n; i++)
	{
		getline(&movie, &buffsize, fp);
		getline(&buff, &buffsize, fp);
		int j, id = 0;
		List temp = NULL;
		nr = atoi(buff);
		for(j = 0; j < nr; j++)
		{
			getline(&actor, &buffsize, fp);
			List tmp = *l;
			bool ok = false;
			while(tmp)
			{
				if(strcmp(actor, tmp->name) == 0)
				{
					ok = true;
					temp = addLast(temp, tmp->id, actor);
					cont--;
					break;
				}
				tmp = tmp->next;
			}
			if(!ok)
				temp = addLast(temp, cont, actor);
			cont++;
		}
		List x = temp;
		while(x)
		{
			List y = x->next;
			while(y)
			{
				graph = insertEdge(graph, x->id, y->id, y->name);
				y = y->next;
			}
			x = x->next;

		}	
		freeList(temp);
	}
	getline(&buff, &buffsize, fp);
	strcpy(actor1, buff);
	getline(&buff, &buffsize, fp);	
	strcpy(actor2, buff);
	fclose(fp);
	free(buff);
	free(actor);
	free(movie);
	return graph;
}

void cerinta1(Graph graph, char* in, char* out)
{
	int *visited = calloc(graph->V, sizeof(int));
	int x = elemConexe(graph, visited);
	FILE *fp = fopen(out,"w");
	fprintf(fp,"%d\n",x);	
	fclose(fp);
	free(visited);
}

void cerinta2(Graph g, List l, char* in, char* out,
				 char* actor1, char* actor2)
{
	size_t buffsize = 32;
	bool visited[g->V];
	int i, ok1 = 0, ok2 = 0;
	List temp = l;
	while(temp != NULL)
	{
		if(!strcmp(actor1, temp->name))
			ok1 = temp->id;
		if(!strcmp(actor2, temp->name))
			ok2 = temp->id;
		if(ok1 && ok2)
			break;
		temp = temp->next;
	}
	int start = ok1, end = ok2, dist[g->V];
	for(i = 0; i < g->V; i++)
	{ 
		visited[i] = false;
		dist[i] = 0;
	}
	Queue q = NULL;
	visited[start] = 1;
	q = enqueue(q, start);
	while(!(isEmptyQueue(q)))
	{
		start = first(q);
		q = dequeue(q);
		List temp = g->adjLists[start];
		while(temp != NULL)
		{
			if(!visited[temp->id])
			{
				visited[temp->id] = true;
				dist[temp->id] = dist[start] + 1;
				q = enqueue(q, temp->id);
			}
			temp = temp->next;
		}
	}	
	freeQueue(q);
	FILE *fp = fopen(out, "w");
	fprintf(fp,"%d\n", dist[end]);

	fclose(fp);
}

void cerinta3Helper(Graph g, int u, bool *visited, int *disc,
					 int *low, int *parent, bool *ap)
{
	static int time = 0;
	int children = 0;
	visited[u] = true;
	disc[u] = low[u] = ++time;
	List i;
	for(i = g->adjLists[u]; i != NULL; i = i->next)
	{
		int v = i->id;
		if(!visited[v])
		{
			children++;
			parent[v] = u;
			cerinta3Helper(g, v, visited, disc, low, parent, ap);
			if(low[u] > low[v])
				low[u] = low[v];
			if(parent[u] == -1 && children > 1)
				ap[u] = true;
			if(parent[u] != -1 && low[v] >= disc[u])
				ap[u] = true;
		}
		else
			if(v != parent[u])
				if(low[u] > low[v])
					low[u] = low[v];
	}
}

void cerinta3(Graph g, char* in, char* out)
{
	int i;
	int *low, *parent, *disc;
	low = parent = disc = malloc(g->V*sizeof(int));
	bool *visited, *ap; 
	visited = ap = malloc(g->V*sizeof(bool));
	for(i = 0; i < g->V; i++)
	{
		parent[i] = -1;
		visited[i] = false;
		ap[i] = false;
	}
	for(i = 0; i < g->V; i++)
		if(visited[i] == false)
			cerinta3Helper(g, i, visited, disc, low, parent, ap);
	FILE *fp = fopen(out, "w");
	for(i = 0; i < g->V; i++)
		if(ap[i] == true)
			fprintf(fp, "%s", g->adjLists[i]->name);
	free(low);
	free(visited);
	fclose(fp);
}

int main(int argc, char** argv)
{
	size_t buffsize = 32;
	Graph g;
	List l;
	char **actor1 = malloc(sizeof(char*));
	char **actor2 = malloc(sizeof(char*));
	*actor1 = malloc(buffsize*sizeof(char));
	*actor2 = malloc(buffsize*sizeof(char));
	g = GraphBuilder(g, &l, argv[2], argv[3], *actor1, *actor2);
	if(!strcmp(argv[1],"-c1"))
		cerinta1(g, argv[2], argv[3]);
	if(!strcmp(argv[1], "-c2"))
		cerinta2(g,l, argv[2], argv[3], *actor1, *actor2);
	if(!strcmp(argv[1], "-c3"))
		cerinta3(g, argv[2], argv[3]);
	free(*actor1);
	free(*actor2);
	free(actor1);
	free(actor2);
	l = freeList(l);
	g = freeGraph(g);
	return 0;
}