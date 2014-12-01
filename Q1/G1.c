#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct edge {
	int vertex_id;
	int weight;
	struct edge *next;
};

struct vertex {
	int vertex_id;
	struct edge *edges_list;
	struct edge *last_edge;
	int total_edges;
};


void add_to_vertex(struct vertex *v, int random_vertex, int random_weight) {

	struct edge *last = NULL;

	if (v->edges_list == NULL) {
                v->edges_list = (struct edge *)malloc(sizeof(struct edge));
                v->edges_list->vertex_id = random_vertex;
                v->edges_list->weight = random_weight;
                v->edges_list->next = NULL;
    
                v->last_edge = v->edges_list;
                v->total_edges++;
    
        } else {
		last = v->last_edge;
		last->next = (struct edge *)malloc(sizeof(struct edge));
		last->next->vertex_id = random_vertex;
		last->next->weight = random_weight;
		last->next->next = NULL;

		last = last->next;
		v->last_edge = last;
		v->total_edges++;
	}	
}
 
void add_edges(struct vertex V[]) {

   time_t t;
   int i,j;
   int random_vertex = 6000, random_weight;
   struct edge *curr_edge=NULL;
	
   /* Intializes random number generator */
   srand((unsigned) time(&t));

    for (i=0;i<5000;i++) {	
	printf("\n\nVertex:%d connected to ",i);
   	/* Print 6 random numbers from 0 to 50 */
   	for( j = 0 ; j < 6 ; j++ ) {
		if (V[i].total_edges<6) {
			do {
				random_vertex = rand()%5000; 
			} while(random_vertex == i || V[random_vertex].total_edges >= 6);
        	        random_weight = rand()%50 + 1; // We don't want the weight to be 0.
	
			add_to_vertex(V+i,random_vertex, random_weight);
			add_to_vertex(V+random_vertex, V[i].vertex_id, random_weight);
		}
   	}

	printf(" %d ",V[i].total_edges);
	curr_edge = V[i].edges_list;
	for(j =0; j<6 ;j++) {
		printf(" V:%d, W:%d \t",curr_edge->vertex_id, curr_edge->weight);
		curr_edge = curr_edge->next;
	}
    }
}

int main() {

	struct vertex v[5000];
	int i;
	for (i=0;i<5000;i++) {
		v[i].vertex_id = i;
		v[i].total_edges =0;
		v[i].last_edge = NULL;
		v[i].edges_list = NULL;
	}

		add_edges(v);
	return 0;
}

