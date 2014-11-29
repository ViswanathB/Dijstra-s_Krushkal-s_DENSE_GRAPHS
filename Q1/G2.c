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
   int random_vertex, random_weight;
   struct edge *curr_edge=NULL;
	
   /* Intializes random number generator */
   srand((unsigned) time(&t));

    for (i=0;i<5000;i++) {	
	//printf("\nVertex:%d connected to ",i);
   	/* Print 6 random numbers from 0 to 50 */
   	for( j = 0 ; j < 1000 ; j++ ) {
		if (V[i].total_edges<1000) {
			random_vertex = rand()%5000;// make sure this is not same as current vertex ??
    
        	        random_weight = rand()%50 + 1; // We don't want the weight to be 0.
	
			add_to_vertex(V+i,random_vertex, random_weight);
			add_to_vertex(V+random_vertex, V[i].vertex_id, random_weight);
		}
   	}

	/* curr_edge = V[i].edges_list;
	for(j =0; j<1000 ;j++) {
		printf(" Vertex:%d, Weight:%d \t",curr_edge->vertex_id, curr_edge->weight);
		curr_edge = curr_edge->next;
	}
	*/
    }
}

void print_list_for_vertex (int vertex, struct edge *edge_list) {
	
	struct edge *curr_edge = edge_list;
	printf("\n Vertex %d connected to\n",vertex);
	while(curr_edge!=NULL) {
                printf(" V:%d,W:%d\t",curr_edge->vertex_id, curr_edge->weight);
                curr_edge = curr_edge->next;
        }	
	
}

int main() {

	struct vertex v[5000];
	//struct edge *curr_edge;

	int i,j;
	for (i=0;i<5000;i++) {
		v[i].vertex_id = i;
		v[i].total_edges =0;
		v[i].last_edge = NULL;
		v[i].edges_list = NULL;
	}

		add_edges(v);
	
	//curr_edge = v[0].edges_list;
	for (i =0;i<5000;i++) 
		print_list_for_vertex(i, v[i].edges_list);
	 //print_list_for_vertex(0, v[0].edges_list);
	 //print_list_for_vertex(500, v[500].edges_list);
	 //print_list_for_vertex(2500, v[2500].edges_list);
	
	return 0;
}

