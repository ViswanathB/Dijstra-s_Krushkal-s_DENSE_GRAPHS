#include <stdio.h>
#include <stdlib.h>

struct heap {
	int H[5001];
	int D[5001];
	int next_element_index;
};

int print_heap(struct heap *h) {
    
        int i; 
        for(i=1;i < h->next_element_index ; i++) {
                printf("\t%d",h->H[i]);
        }
}

int swap (int *a, int *b) {
    
        int temp;
    
        temp = *a; 
        *a = *b; 
        *b = temp;
}

int max_of_three(int a, int b, int c) {

	int max = a;
	
	if (b>max) max = b;
	if (c>max) max = c;

	return max;
}


//This filters the heaviest member place from the bottom of the heap 
// to the root to its right position.
int delete_maxofheap (struct heap *h) {

	h->H[1] = h->H[h->next_element_index-1];
        h->H[h->next_element_index] = 0;
        h->next_element_index--;
        //printf("\n next_element_index :%d\n",h->next_element_index);
	
	int temp=1, maxofthree;
	while (temp <= (h->next_element_index/2)) {
		if ((h->H[temp] < h->H[2*temp]) || (h->H[temp] < h->H[2*temp+1])) {
			maxofthree = max_of_three(h->H[temp],h->H[2*temp], h->H[2*temp+1]);
			if (h->H[temp] == maxofthree) {
				// we don't have to do anything since H[temp] is the largest
				break;
			} else if (h->H[temp*2] == maxofthree){
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

}

/*
int delete_maxofheap (struct heap *h) {
	
	h->H[1] = h->H[h->next_element_index-1];
	h->H[h->next_element_index] = 0;
	h->next_element_index--;
	//printf("\n next_element_index :%d\n",h->next_element_index);
	
	int temp = 1;// count = 0;
	while (temp < h->next_element_index) {
		if (((h->H[temp] < h->H[2*temp]) || (h->H[temp] < h->H[2*temp+1])) &&
					(2*temp+1 <= h->next_element_index)) {
			//printf("\n yes it is more");count++; if (count == 10) break;
			if (h->H[temp] < h->H[2*temp]) { //x<y
				if (h->H[2*temp] > h->H[2*temp+1]) {//y>z
					swap(h->H+temp,h->H+2*temp);
					temp = temp*2;
				} else if (h->H[2*temp] < h->H[2*temp+1]) {//y<z
					swap(h->H+temp, h->H+2*temp+1);
				}
			} else if ((h->H+temp < h->H+2*temp+1)) {//x>=y but x<z
				swap(h->H+temp,h->H+2*temp+1);
				temp = temp*2;
			}
		} else {
			if (2*temp+1 == h->next_element_index) {// We have only 2 elements
				if(h->H[1] < h->H[2]) {
					swap(h->H+1,h->H+2);
				}
			}
			break;
		}	
	}
}
*/

//This filters the heap upwards to the 
//root, the aim to get the new element to a position 
//where it is less than all its children
	
void maxify_heap(struct heap *h) {

	int temp = h->next_element_index;
	
	while (temp>2 && temp!=1) {
		if (h->H[temp] > h->H[temp/2]) {
			swap(h->H+temp, h->H+(temp/2));
		} //else if (h->H[temp] > h->H[(temp/2)+1]) { 
		  //	swap(h->H+temp, h->H+(temp/2)+1);
		else break;
		temp = temp/2;
	}
 	
	if (temp == 2) { 
           	if(h->H[2] > h->H[1])
           		swap(h->H+1,h->H+2);
	}
}

int insert_inheap(struct heap *h, int a) {
	
	h->H[h->next_element_index] = a;
	//print_heap(h);
	if (h->next_element_index>2)
 		maxify_heap(h);
	else if (h->next_element_index == 2){
		if(h->H[2] > h->H[1])
			swap(h->H+1,h->H+2);
	}
	h->next_element_index++;
	//print_heap(h);
}


int main() {

	struct heap my_heap;
	//int H[5001], D[5001];
	int i,a=1;

	my_heap.next_element_index = 1;
	for (i=1;i<5001;i++) {
		my_heap.H[i] = 0;
		my_heap.D[i] = 0;
	}

	while(a) {
		printf("enter");
		scanf("%d",&a);
		if (a!=0) {
			insert_inheap(&my_heap, a);
		}
	}

	print_heap(&my_heap);
	a =1;
	while(a) {
		printf("\n Enter deletion");
		scanf("%d",&a);
		if(a!=0) {
			if (my_heap.next_element_index >1)
				delete_maxofheap(&my_heap);
			else {
				printf("\n All heap elements have been deleted");
				break;
			}
		}
		print_heap(&my_heap);
	}

	return 0;	
}


