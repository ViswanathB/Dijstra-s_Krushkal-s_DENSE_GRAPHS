#include "Heap.h"

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
