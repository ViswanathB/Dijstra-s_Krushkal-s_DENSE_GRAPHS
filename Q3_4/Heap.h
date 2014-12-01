#include <stdio.h>
#include <stdlib.h>

struct heap {
	int H[5001];
	int D[5001];
	int next_element_index;
};

struct heap_element {
	int vertex_id;
	int flow;
};

struct fringes_heap {
	struct heap_element H[5001];
	int next_element_index;
};

int print_heap(struct heap *h) {
    
        int i; 
        for(i=1;i < h->next_element_index ; i++) {
                printf("\t%d",h->H[i]);
        }
}

int swap (struct heap_element *a, struct heap_element *b) {
    
        struct heap_element temp;
    
        temp.vertex_id = a->vertex_id;
	temp.flow = a->flow;
 
        a->vertex_id = b->vertex_id; 
        a->flow = b->flow;

	b->vertex_id = temp.vertex_id;
	b->flow = temp.flow;
}

int max_of_three(int a, int b, int c) {

	int max = a;
	
	if (b>max) max = b;
	if (c>max) max = c;

	return max;
}


//This filters the heaviest member place from the bottom of the heap 
// to the root to its right position.
struct heap_element delete_maxofheap (struct fringes_heap *h) {

	struct heap_element max_ele;
	max_ele.flow = h->H[1].flow;
	max_ele.vertex_id = h->H[1].vertex_id;
	
	h->H[1].flow = h->H[h->next_element_index-1].flow;
	h->H[1].vertex_id = h->H[h->next_element_index-1].vertex_id;

        h->H[h->next_element_index-1].flow = 0;
	h->H[h->next_element_index-1].vertex_id = 0;
        
	h->next_element_index--;
        //printf("\n next_element_index :%d\n",h->next_element_index);
	
	int temp=1, maxofthree;
	while (temp <= (h->next_element_index/2)) {
		if ((h->H[temp].flow < h->H[2*temp].flow) || (h->H[temp].flow < h->H[2*temp+1].flow)) {
			maxofthree = max_of_three(h->H[temp].flow,h->H[2*temp].flow, h->H[2*temp+1].flow);
			if (h->H[temp].flow == maxofthree) {
				// we don't have to do anything since H[temp] is the largest
				break;
			} else if (h->H[temp*2].flow == maxofthree){
				swap(h->H+temp, h->H+temp*2); 
				temp = temp*2;
			} else { // this means h->H+temp*2+1 is the largest
				swap(h->H+temp, h->H+temp*2+1);
				temp = temp*2+1;
			}	
		} else {
			break;
		}
	}

	return max_ele;
}

//This filters the heap upwards to the 
//root, the aim to get the new element to a position 
//where it is less than all its children
	
void maxify_heap(struct fringes_heap *h, int index_to_start_from) {

	int temp = index_to_start_from;
	
	while (temp>2 && temp!=1) {
		if (h->H[temp].flow > h->H[temp/2].flow) {
			swap(h->H+temp, h->H+(temp/2));
		} else {
	 		break;
		}
		temp = temp/2;
	}
 	
	if (temp == 2) { 
           	if(h->H[2].flow > h->H[1].flow)
           		swap(h->H+1,h->H+2);
	}
}

int insert_inheap(struct fringes_heap *h, int vertex, int vertex_d) {
	
	h->H[h->next_element_index].flow = vertex_d;
	h->H[h->next_element_index].vertex_id = vertex;
	//print_heap(h);
	if (h->next_element_index>2)
 		maxify_heap(h, h->next_element_index);
	else if (h->next_element_index == 2){
		if(h->H[2].flow > h->H[1].flow)
			swap(h->H+1,h->H+2);
	}
	h->next_element_index++;
	//print_heap(h);
}

void update_inheap(struct fringes_heap *h, int vertex_id, int vertex_d) {

	int i = 1;

	while (i < h->next_element_index) {
		if (h->H[i].vertex_id == vertex_id) {
			//printf("\n found vertex %d with %d, updating with %d", vertex_id, h->H[i].flow, vertex_d);
			h->H[i].flow = vertex_d;
			maxify_heap(h,i);
			break;
		}
		i++;
	}
}
