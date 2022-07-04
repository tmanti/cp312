#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEAP_CAPACITY 39

typedef enum node_type{
    leaf,
    intnode
} NTYPE;

typedef struct tree_node {
    NTYPE type;
    char val;
    int freq;
    TNODE *left;
    TNODE *right;
} TNODE;

typedef TNODE KEYTYPE;  // key type
typedef int DATA;     // data type 

typedef struct heap_node { // data element ds for binary heap
  KEYTYPE key;
  DATA frequency;
} HNODE;

typedef struct heap {  
  unsigned int size;     // the current heap size, i.e., heap node count
  char *hna;  // pointer pointing the heap node array
} HEAP;

HEAP *new_heap();
void insert(HEAP *heap, HNODE new_node);
HNODE extract_min(HEAP *heap);

int main(int argc, char *argv[])
{

}

int cmp(KEYTYPE a, KEYTYPE b) {
    // your implementation
    if(a<b){
        return -1;
    } else if (a>b){
        return 1;
    } else {
        return 0;
    }
}

void heap_swap(char *a, char *b){
    char  temp;
    temp = *a;
    *a   = *b;
    *b   = temp;
    return;
}

int heapify_up(HEAP *source, int a){
    HNODE *p = source->hna;
	
	int i = a;
	int j = (i-1)/2;

	while(i != 0){
		if(cmp((p+j)->key, (p+i)->key) == 1){
			heap_swap((p+j), (p+i));
			i = j;
			j = (i-1)/2;
		} else {
			break;
		}
	}
    return i;
}

int heapify_down(HEAP *source, int a){
    HNODE *p = source->hna;
	
	int i = a;
    int l = 2*i+1;
    int r = 2*i+2;
	int j;

	while(i < source->size){
        if(r>=source->size){
            if(l>= source->size){
                break;
            }
            else {
                j = l;
            }
        } else {
            j = (cmp((p+l)->key, (p+r)->key) == -1)?l:r;
        }
        //printf("hd: %d %d %d\n", (p+j)->key, (p+i)->key, cmp((p+j)->key, (p+i)->key));
		if(cmp((p+j)->key, (p+i)->key) == -1){
			heap_swap((p+j), (p+i));
			i = j;
			l = 2*i+1;
            r = 2*i+2;
		} else {
			break;
		}
	}
    return i;
}

HEAP *new_heap(){
    HEAP *hp = (HEAP *) malloc(sizeof(HEAP));
    hp->size = 0;
    hp->hna = (HNODE*)malloc(sizeof(HNODE)*HEAP_CAPACITY);
    return hp;
}

void insert(HEAP *heap, HNODE new_node)
{
    HNODE *p = heap->hna;
    *(p+heap->size) = new_node;
    heap->size++;
    heapify_up(heap, heap->size-1);
    return;
}

HNODE extract_min(HEAP *heap)
{
    // your implementation
    //printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    //display_heap2(heap);
    HNODE *p = heap->hna;
    heap_swap(p, p+heap->size-1);
    HNODE res = *(p+heap->size-1);
    //display_heap2(heap);
    heap->size--;
   // display_heap2(heap);
    heapify_down(heap, 0);
   // display_heap2(heap);
    //printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    return res;
}