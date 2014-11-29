#include <stdbool.h>

 int total_number_of_edges=0;

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
	int flow;
	int color;
	int parent;
};

struct graph {
        struct vertex v[5000];
        int status[5000];
        int d[5000];
        int dad[5000];
};

struct alledges {
	int vertex1;
	int vertex2;
	int weight;	
};

bool add_to_vertex(struct vertex *v, int random_vertex, int random_weight);

struct alledges alledges_heap[4000000];
int alledges_heap_next_element = 1;

struct vertex mast[5000];//maximum spaning tree consists of n-1 edges
int mast_edges=0;

int findset[5000];
int rank[5000];

int minimum (int a, int b) {

        if(a>=b) return b;
        else if(a<b) return a;
}

void swap_nodes (struct alledges *x, struct alledges *y) {
	
	struct alledges *temp;
	temp = (struct alledges *)malloc(sizeof(struct alledges));
	
	temp->vertex1 = x->vertex1;
	temp->vertex2 = x->vertex2;
	temp->weight = x->weight;

	x->vertex1 = y->vertex1;
	x->vertex2 = y->vertex2;
	x->weight = y->weight;

	y->vertex1 = temp->vertex1;
        y->vertex2 = temp->vertex2;
        y->weight = temp->weight;	
}

void maxify_heap_k() {

	int temp = alledges_heap_next_element;
	
	while (temp>2 && temp!=1) {
                if (alledges_heap[temp].weight > alledges_heap[temp/2].weight) {
                        swap_nodes(alledges_heap+temp, alledges_heap+(temp/2));
                } else {
                        break;
                }
                temp = temp/2;
        }
    
        if (temp == 2) { 
                if(alledges_heap[2].weight > alledges_heap[1].weight)
                        swap_nodes(alledges_heap+1,alledges_heap+2);
        }
}

void insert_alledges_heap(int vertex1, int vertex2, int weight) {
	
	alledges_heap[alledges_heap_next_element].vertex1 = vertex1;
	alledges_heap[alledges_heap_next_element].vertex2 = vertex2;
	alledges_heap[alledges_heap_next_element].weight = weight;
	
	if (alledges_heap_next_element > 2) {
                maxify_heap_k();
        } else if (alledges_heap_next_element == 2) {
                if(alledges_heap[2].weight > alledges_heap[1].weight)
                        swap_nodes(alledges_heap+1,alledges_heap+2);
        }

        alledges_heap_next_element++;
	return;
}

struct alledges delete_maxofheapmast() {
	struct alledges max_ele;
        
	max_ele.weight = alledges_heap[1].weight;
        max_ele.vertex1 = alledges_heap[1].vertex1;
	max_ele.vertex2 = alledges_heap[1].vertex2;

	alledges_heap[1].vertex1 = alledges_heap[alledges_heap_next_element-1].vertex1;
	alledges_heap[1].vertex2 = alledges_heap[alledges_heap_next_element-1].vertex2;
	alledges_heap[1].weight = alledges_heap[alledges_heap_next_element-1].weight;
	
	alledges_heap[alledges_heap_next_element-1].vertex1 = 0;
	alledges_heap[alledges_heap_next_element-1].vertex2 = 0;
	alledges_heap[alledges_heap_next_element-1].weight = 0;

        alledges_heap_next_element--;
        //printf("\n next_element_index :%d\n",h->next_element_index);
    
        int temp=1, maxofthree;
        while (temp <= (alledges_heap_next_element/2)) {
                if ((alledges_heap[temp].weight < alledges_heap[2*temp].weight) || 
				(alledges_heap[temp].weight < alledges_heap[2*temp+1].weight)) {
                        maxofthree = max_of_three(alledges_heap[temp].weight,alledges_heap[2*temp].weight, alledges_heap[2*temp+1].weight);
                        if (alledges_heap[temp].weight == maxofthree) {
                                // we don't have to do anything since H[temp] is the largest
                                break;
                        } else if (alledges_heap[temp*2].weight == maxofthree){
                                swap_nodes(alledges_heap+temp, alledges_heap+temp*2); 
                                temp = temp*2;
                        } else { // this means h->H+temp*2+1 is the largest
                                swap_nodes(alledges_heap+temp, alledges_heap+temp*2+1);
                                temp = temp*2+1;
                        }    
                } else {
                        break;
                }
        }

	return max_ele;
}

int parent(int vertex) {
	
	if (findset[vertex] == -1) return -1;

	while ( findset[vertex]!=vertex ) {
		vertex = findset[vertex];	
	}

}

bool cycle(struct alledges max_ele) {
	
	int parent1 = parent(max_ele.vertex1);
	int parent2 = parent(max_ele.vertex2);

	if (parent1 == -1 && parent2 == -1) {
		rank[max_ele.vertex1]++;
		rank[max_ele.vertex2] = 0;
		findset[max_ele.vertex2] = max_ele.vertex1;
		findset[max_ele.vertex1] = max_ele.vertex1; 
	} else if (parent1 == -1 && parent2 > -1) {
		findset[max_ele.vertex1] = parent2;
		rank[parent2]++;
		rank[parent1] = 0;
	} else if (parent1 > -1 && parent2 == -1) {
		findset[max_ele.vertex2] = parent1;
		rank[parent1]++;
		rank[parent2] = 0;
	} else if (parent1 == parent2){
		return true;
	} else {
		if(rank[parent1] >= rank[parent2]) {
			findset[parent2] = parent1;
			rank[parent1]++;
		} else {
			findset[parent1] = parent2;
			rank[parent2]++;
		}

	}
	
	return false;
}

void addtomast(struct alledges max_ele) {

	add_to_vertex(mast+max_ele.vertex1,max_ele.vertex2,max_ele.weight);
	add_to_vertex(mast+max_ele.vertex2,max_ele.vertex1,max_ele.weight);

}

void construct_mast() {
	int i=0;
	struct alledges max_ele;
	int deleted_edges=0;

	int mastcount = 0;
	for(i=0 ; i < alledges_heap_next_element ; i++) {
		max_ele = delete_maxofheapmast();
		//printf("\n%d",max_ele.weight);
		deleted_edges++;
		if (!cycle(max_ele)) {
			mastcount++;
			addtomast(max_ele);
		}
	}

//	printf("\n deleted_edges :%d",deleted_edges);
//	printf("\n Maximum spanning tree nodes:%d", mastcount);

	return;
}

void DFS(int v) {

	struct edge *edgelist = mast[v].edges_list;

	mast[v].color = 1; //grey
	//printf("\n%d",v);
	while (edgelist!=NULL) {
		mast[edgelist->vertex_id].flow = minimum(mast[v].flow, edgelist->weight);
		
		if (mast[edgelist->vertex_id].color == 0) {
			DFS(edgelist->vertex_id);
			mast[edgelist->vertex_id].parent = v;
		}
		edgelist = edgelist->next;
	} 

}

void krushkals_max_bandwidth(struct graph *g, int src, int dest) {

	//printf("\n total number of edges:%d", total_number_of_edges);

	int i=0;
	struct edge *edgelist = NULL;
	int inserted_edges=0;

	for (i=0;i<5000;i++) {
		edgelist = g->v[i].edges_list;
		while (edgelist != NULL) {
			if ( i < edgelist->vertex_id ) {
				inserted_edges++;
				insert_alledges_heap(i,edgelist->vertex_id,edgelist->weight);
			}
			//printf("\n %d->%d:%d",i,edgelist->vertex_id,edgelist->weight);
			edgelist = edgelist->next;
		}
	}

	//printf("\n alledges_heap_next_element:%d, inserted_Edges:%d",alledges_heap_next_element, inserted_edges);
	
	for (i=0;i<5000;i++) { 
		mast[i].vertex_id = i;
		mast[i].total_edges = 0;
		mast[i].last_edge = NULL;
		mast[i].edges_list = NULL;
		mast[i].color = 0; //white
		mast[i].flow = 2147483640;
		mast[i].parent = -1;
	}
	for (i=0;i<5000;i++) {
		findset[i] = -1;
		rank[i] = -1;
	}
	construct_mast();

	DFS(src);

	printf("\n The maximum flow possible src :%d to dest %d:%d\n\n",src,dest, mast[dest].flow);
	printf("\n Path from destination to source using backtracking \n");
	printf("%d", dest);

	while(src!=dest) {
		dest = mast[dest].parent;
		printf(" ->((%d))-> %d\t",mast[dest].flow,dest);
			
	}
	return;
} 
