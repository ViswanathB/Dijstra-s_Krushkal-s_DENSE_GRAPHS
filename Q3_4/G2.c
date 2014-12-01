#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "Heap.h"
#include "krushkal.h"

bool add_to_vertex(struct vertex *v, int random_vertex, int random_weight) {

	struct edge *last = NULL;

	if (v->vertex_id == random_vertex) return false; //The random vertex generated 
						         //is same as the current vertex
	if (v->edges_list == NULL) {
                v->edges_list = (struct edge *)malloc(sizeof(struct edge));
                v->edges_list->vertex_id = random_vertex;
                v->edges_list->weight = random_weight;
                v->edges_list->next = NULL;
    
                v->last_edge = v->edges_list;
                v->total_edges++;

		return true;
    
        } else {
		last = v->edges_list;

		while (last->next!= NULL) {
			if(last->vertex_id == random_vertex)
				return false;	
			last = last->next;
		}
		last->next = (struct edge *)malloc(sizeof(struct edge));
		last = last->next;
		last->vertex_id = random_vertex;
		last->weight = random_weight;
		last->next = NULL;
		
		v->last_edge = last;
		v->total_edges++;
		return true;
	}	
}
 
void add_edges(struct vertex V[]) {

   time_t t;
   int i,j;
   int random_vertex, random_weight;
   struct edge *curr_edge=NULL;
	
   /* Intializes random number generator */
   srand((unsigned) time(&t));

    for (i=0;i<=4998;i++) {
	random_weight = rand()%1000 + 1;// This is to make sure there is always a path between 2 vertices
					// We are limiting the weights to 1000 for the reason to choose one
					// of these edges only in the worst case. or else, let the actual 
					// maximum bandwidth be consisting edges added below.
	add_to_vertex(V+i,i+1,random_weight);
	add_to_vertex(V+i+1,i,random_weight);	
    } 

    for (i=0;i<5000;i++) {	
   	while(1) {
		if (V[i].total_edges<1000) {
			random_vertex = rand()%5000;// make sure this is not same as current vertex ??
        	        random_weight = rand()%1000000 + 1; // We don't want the weight to be 0.
			if (add_to_vertex(V+i,random_vertex, random_weight)) {
				total_number_of_edges++;
				if(!add_to_vertex(V+random_vertex, V[i].vertex_id, random_weight)) {
					//printf("\n It says it already exists from %d, to %d",random_vertex, V[i].vertex_id);
				}
			}
		} else {
			break;	
		}
   	}
	//if (i==0) 
	//printf("\n edges initially added to %d were :%d", i, V[i].total_edges);

    }
}

void print_list_for_vertex (int vertex, struct edge *edge_list) {
	
	struct edge *curr_edge = edge_list;
	printf("\n Vertex %d connected to",vertex);
	int adjacency=0;
	while(curr_edge!=NULL) {
                //printf(" V:%d,W:%d\t",curr_edge->vertex_id, curr_edge->weight);
                curr_edge = curr_edge->next;
		adjacency++;
        }	
	printf("\t %d vertices",adjacency);
}

int find_max_d_fringes(struct graph *g) {

        int max_d_fringe = -1; // initialising with -1 as we know
                                   // all the edge weights are positive
	int vertex_id = 0;
	int i;

        for (i=0;i<5000;i++) {
                if (g->status[i] == 1) {
                        if (g->d[i] > max_d_fringe) {
                                max_d_fringe = g->d[i];
				vertex_id = i;
                        }
                }
        }
	//printf("\nnext_fringe:%d",vertex_id);
        return vertex_id;
}

/*
int minimum (int a, int b) {

	if(a>=b) return b;
	else if(a<b) return a;
}
*/	
void dijikstra_maxflow(struct graph *g,int src, int dest){

	int i=0;
        struct edge *s_edges;
        int next_fringe=0;

        for (i=0;i<5000;i++) {
                g->status[i] = 0; //unseen
        }

        //printf("\n%d",src);
        g->status[src] = 2; //intree
        g->dad[src]=10000; // Some random number which is not any vertex 
        g->d[src]=0;
        s_edges = g->v[src].edges_list;

        while (s_edges!=NULL) {
                g->status[s_edges->vertex_id] = 1; //making this a fringe
                g->dad[s_edges->vertex_id] = src;
                g->d[s_edges->vertex_id] = s_edges->weight;
                s_edges = s_edges->next;
        }

        int count_vertices = 1; // Let this be the count of vertices in the tree 
                                // we have covered so far
        //printf("\n Fringes\n");
        while (count_vertices<=5000) {
                next_fringe = find_max_d_fringes(g);
                //printf("%d ", next_fringe);
                g->status[next_fringe] = 2;
                count_vertices++;
		//if (next_fringe == dest) break;
    
                s_edges = g->v[next_fringe].edges_list;
   		//if ( dest == next_fringe) printf("\n Our next fringe is actual destination");
 
                while (s_edges!=NULL) {
			if (dest == s_edges->vertex_id) {
				//printf("\n we hit %d again which has max_flow as %d, from %d which has max_flow as %d with an edge %d", dest, g->d[dest], next_fringe,g->d[next_fringe], s_edges->weight);
			}
                       if (g->status[s_edges->vertex_id] == 0) {
                                g->status[s_edges->vertex_id] = 1; //making this a fringe
                                g->dad[s_edges->vertex_id] = next_fringe;
				g->d[s_edges->vertex_id] = minimum(g->d[next_fringe],s_edges->weight);
                        } else if ( g->status[s_edges->vertex_id] == 1 &&  
                                                        (g->d[s_edges->vertex_id] < minimum(g->d[next_fringe],s_edges->weight))) {
                                g->d[s_edges->vertex_id] = minimum(g->d[next_fringe], s_edges->weight);
                                g->dad[s_edges->vertex_id] = next_fringe;
                        }
    
                        s_edges = s_edges->next;
                        if (count_vertices == 5000) { 
				//printf("\n The total number of vertices are 5000"); 
				break; 
			}
                }
        }

        printf("\n The maximum flow possible from:%d -> to:%d is %d\n", src, dest, g->d[dest]);
}

void dijikstra_maxflow_heap(struct graph *g, int src, int dest) {
        int i=0;
        struct edge *s_edges;
        int next_fringe=0;
	
	struct fringes_heap *fringe_heap; 
			
	fringe_heap = (struct fringes_heap *)malloc(sizeof(struct fringes_heap));
	fringe_heap->next_element_index = 1;
	
        for (i=0;i<5000;i++) {
                g->status[i] = 0; //all vertices are unseen for now
	}

	for (i=1;i<5001;i++) {	
		fringe_heap->H[i].vertex_id = 0;
		fringe_heap->H[i].flow = 0;	
        }

        //printf("\n%d",src);
        g->status[src] = 2; //intree
        g->dad[src]=10000; // Some random number which is not any vertex 
        g->d[src]=0;
        s_edges = g->v[src].edges_list;
	int number_of_fringes_inserted=0;

        while (s_edges!=NULL) {
                g->status[s_edges->vertex_id] = 1; //making this a fringe
                g->dad[s_edges->vertex_id] = src;
                g->d[s_edges->vertex_id] = s_edges->weight;
		insert_inheap(fringe_heap, s_edges->vertex_id, g->d[s_edges->vertex_id]); 
		number_of_fringes_inserted++;
		s_edges = s_edges->next;
		
        }

        int count_vertices = 1; // Let this be the count of vertices in the tree 
                                // we have covered so far
	struct heap_element next_fringe_heap_ele;
	int fringes_out_of_heap=1;
        //printf("\n Fringes\n");
        while (count_vertices<=5000) {
                //next_fringe = find_max_d_fringes(g);
		next_fringe_heap_ele = delete_maxofheap(fringe_heap);
		next_fringe = next_fringe_heap_ele.vertex_id;
		if (next_fringe == dest) break;
		/*if (next_fringe == ele.vertex_id) {
			//printf("\n heap_works for fringe:%d",ele.vertex_id);
			fringes_out_of_heap++;
			//printf("\n fringe is out%d",fringes_out_of_heap);
			if (fringes_out_of_heap == 4999) printf("\n all are out");
		} else {
			//printf("\nnext_fringe flow : %d and maxheap :%d",g->d[next_fringe],g->d[ele.vertex_id]);
		}*/
		
		//next_fringe = ele.vertex_id;

                g->status[next_fringe] = 2;
                count_vertices++;

                s_edges = g->v[next_fringe].edges_list;
                //if ( dest == next_fringe) printf("\n Our next fringe is actual destination");

                while (s_edges!=NULL) {
                        if (dest == s_edges->vertex_id) {
                                //printf("\n we hit %d again which has max_flow as %d, from %d which has max_flow as %d with an edge %d", dest, g->d[dest], next_fringe,g->d[next_fringe], s_edges->weight);
                        }
                       if (g->status[s_edges->vertex_id] == 0) {
                                g->status[s_edges->vertex_id] = 1; //making this a fringe
				g->dad[s_edges->vertex_id] = next_fringe;
                                g->d[s_edges->vertex_id] = minimum(g->d[next_fringe],s_edges->weight);
				insert_inheap(fringe_heap, s_edges->vertex_id, g->d[s_edges->vertex_id]);
				number_of_fringes_inserted++;
                        } else if ( g->status[s_edges->vertex_id] == 1 &&
                                                        (g->d[s_edges->vertex_id] < minimum(g->d[next_fringe],s_edges->weight))) {
                                g->d[s_edges->vertex_id] = minimum(g->d[next_fringe], s_edges->weight);
                                g->dad[s_edges->vertex_id] = next_fringe;
				update_inheap(fringe_heap, s_edges->vertex_id, g->d[s_edges->vertex_id]);
                        }

                        s_edges = s_edges->next;
                        if (count_vertices == 5000) { //printf("\n The total number of vertices are 5000"); 
				break; }
                }
        }

        printf("\n The maximum flow possible from:%d -> to:%d is %d\n", src, dest, g->d[dest]);
	//printf("\n The total number of fringes inserted :%d", number_of_fringes_inserted);
	//printf("\n The total number of fringes out_of_heap:%d, still left:%d", fringes_out_of_heap, fringe_heap->next_element_index );
}

int main() {


	struct graph *G1;
	G1 = (struct graph *)malloc(sizeof(struct graph));
 
	int i,j;
	for (i=0;i<5000;i++) {
		G1->v[i].vertex_id = i;
		G1->v[i].total_edges =0;
		G1->v[i].last_edge = NULL;
		G1->v[i].edges_list = NULL;
		G1->status[i]=10;// not 0,1,2 which are used in dijikstra
		G1->dad[i]=10000; // not any from 1-5000
		G1->d[i] =0; 
	}

		add_edges(G1->v);
	
	printf("\n The graph generated succesfully and data structures initialized properly");
	/*for (i =0;i<5000;i++) 
		print_list_for_vertex(i, G1->v[i].edges_list);
	*/
	
	int src;
	int dest;

	//printf("RAND_MAX%d",RAND_MAX);
	/*if (src == dest) { 
		printf("\n The maximum flow is 0"); 
		return 0; 
	}*/
	//krushkals_max_bandwidth(G1, 200, 2000);

        clock_t begin, end;
	double time_spent;

	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	int final_destination, sets;
	for(sets=0;sets<5;sets++) {

		src = rand()%5000;
		dest = rand()%5000;
		
		final_destination = dest;

		printf("\n Source and destination generated are %d and %d respectively", src, dest);
		if (src == dest) { 
               	 	printf("\n Since source is equal to destination the maximum flow is 0"); 
                	//return 0;
			continue;  
        	}
		
		printf("\n\nTHIS IS WITHOUT HEAP structure for fringes--------------\n");
		
		begin = clock();
		dijikstra_maxflow(G1, src, dest);// Dijikstra's without heap	
		end = clock();
		printf(" Running time is %f sec", (double)(end-begin)/CLOCKS_PER_SEC);

		int no_of_vertices=0;
		printf("\n Path from destination to source using backtracking\n");
                printf("\n %d",dest);
                while (1) {
                        no_of_vertices++; 
                        printf(" ->((%d))-> %d\t",G1->d[dest], G1->dad[dest]);
                        dest = G1->dad[dest];
                        if (dest == 10000 || no_of_vertices == 4999 || dest == src) break;
                }
	
		for (i=0;i<5000;i++) {
                        G1->status[i]=10;
                        G1->dad[i]=10000;
                        G1->d[i]=0;
                }

		dest = final_destination;
		//dijikstra_maxflow_heap(G1, src, dest);// Dijikstra's with heap 
        	printf("\n\nTHIS IS USING HEAP struture for fringes-----------------\n");
		begin = clock();
		dijikstra_maxflow_heap(G1, src, dest);// Dijikstra's with heap	
		end = clock();
		printf(" Running time is %f sec\n", (double)(end-begin)/CLOCKS_PER_SEC);
 		no_of_vertices=0;
		printf("\n Path from destination to source using backtracking\n");
		printf("\n %d", dest);
		while (1) {
			no_of_vertices++; 
			printf(" ->((%d))-> %d\t",G1->d[dest], G1->dad[dest]);
			dest = G1->dad[dest];
			if (dest == 10000 || no_of_vertices == 4999 || dest == src) break;
		}

	   	for (i=0;i<5000;i++) {
                        G1->status[i]=10;
                        G1->dad[i]=10000;
                        G1->d[i]=0;
                }
                dest = final_destination;
		printf("\nTHIS IS USING modified Krushkal's---------------------\n");
		for(i=0;i<4000000;i++) { 
			alledges_heap[i].vertex1 = 0;
			alledges_heap[i].vertex2 = 0;
			alledges_heap[i].weight = 0;
		}
		//all_edges_heap_next_element = 1;
		krushkals_max_bandwidth(G1, src, dest);	
		
		//printf("\ndo you want to calculate for another destination.");
		//scanf("%d",&dest);
		printf("\n ----------------------------------------------------------------------------------------------------------\n");
		if(sets<4)printf("\n Calculating for next set of pair");
		for (i=0;i<5000;i++) {
			G1->status[i]=10;
			G1->dad[i]=10000;
			G1->d[i]=0;
		}
	} 
	return 0;
}

