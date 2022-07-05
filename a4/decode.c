#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HEAP_CAPACITY 39

typedef enum node_type{
    LEAF,
    INT
} NTYPE;

typedef struct tree_node {
    NTYPE type;
    char val;
    int freq;
    struct tree_node *left;
    struct tree_node *right;
} TNODE;

typedef int KEYTYPE;  // key type
typedef TNODE DATA;     // data type 

typedef struct heap_node { // data element ds for binary heap
  KEYTYPE key;
  DATA *node;
} HNODE;

typedef struct heap {  
  unsigned int size;     // the current heap size, i.e., heap node count
  HNODE *hna;  // pointer pointing the heap node array
} HEAP;

HEAP *new_heap();
void insert_internal(HEAP *heap, TNODE *node1, TNODE *node2);
void insert_freq(HEAP *heap, char c, int freq);
void insert(HEAP *heap, HNODE new_node);
HNODE extract_min(HEAP *heap);

char freq_chars[39] = {
    ' ',
    ',',
    '.',
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z'
};

int get_char_index(char c){
    if(isspace(c) && c !='\r'){
        return 0;
    } else if (c == ','){
        return 1;
    } else if(c == '.'){
        return 2;
    } else if(c >= '0' && c <= '9'){
        int a = (int) c - '0';
        return 3+a;
    } else {
        if(c >= 'A' && c <= 'Z'){
            int a = (int) c - 'A';
            return 13+a;
        } else if(c>= 'a' && c<= 'z') {
            int a = (int) c -'a';
            return 13+a;
        }
    }
    return -1;
}

void count(char c, int counts[HEAP_CAPACITY]){
    int index = get_char_index(c);
    if(index != -1){
        counts[index]++;
    }
}

int main(int argc, char *argv[])
{
    int counts[HEAP_CAPACITY];

    FILE *freq_in = fopen("frequency.txt", "r");
    char in;
    int freq;
    for(int i = 0; i < HEAP_CAPACITY; i++){
        fscanf(freq_in, "%c:%d\n", &in, &freq);
        counts[get_char_index(in)] = freq;
    }
    fclose(freq_in);

    HEAP *h = new_heap();

    for(int i = 0; i < HEAP_CAPACITY; i++){
        insert_freq(h, freq_chars[i], counts[i]);
    }

    while(h->size > 1){
        HNODE n1 = extract_min(h);
        HNODE n2 = extract_min(h);
        insert_internal(h, n1.node, n2.node);
    }

    TNODE *huffman_root = extract_min(h).node;

    FILE *decoded = fopen("decoded.txt", "w");
    FILE *bin = fopen("compressed.bin", "rb");

    TNODE *curr = huffman_root;
    char c, bit;
    char mask = 1 << 7;
    int i;
    while(!feof(bin)){
        c = fgetc(bin);
        for(i = 0; i<8; i++){
            bit = c & mask;
            c = c << 1;
            if(bit==0){
                curr = curr->left;
                if(curr->type == LEAF){
                    fputc(curr->val, decoded);
                    curr = huffman_root;
                }
            } else {
                curr = curr->right;
                if(curr->type == LEAF){
                    fputc(curr->val, decoded);
                    curr = huffman_root;
                }
            }
        }
    }

    fclose(bin);
    fclose(decoded);

    return 0;
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

void heap_swap(HNODE *a, HNODE *b){
    HNODE  temp;
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

void insert_internal(HEAP *heap, TNODE *node1, TNODE *node2){
    TNODE *new_node = (TNODE*)malloc(sizeof(TNODE));
    int freq = node1->freq + node2->freq;
    new_node->freq = freq;
    TNODE *small = (node1->freq < node2->freq)?node1:node2;
    TNODE *big = (node1->freq > node2->freq)?node1:node2;

    new_node->left = big;
    new_node->right = small;

    new_node->type = INT;
    new_node->val = NULL;

    HNODE hnode = {freq, new_node};

    insert(heap, hnode);
}

void insert_freq(HEAP *heap, char c, int freq){
    TNODE *new_node = (TNODE*)malloc(sizeof(TNODE));
    new_node->type = LEAF;
    new_node->freq = freq;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->val = c;

    HNODE hnode = {freq, new_node};
    insert(heap, hnode);
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