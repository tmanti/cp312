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

int main(int argc, char *argv[])
{
    TNODE *huffman_root = (TNODE*)malloc(sizeof(TNODE));
    huffman_root->type = INT;

    FILE *codes_in = fopen("codes.txt", "r");
    char in;
    char code[15];
    char line[100];
    for(int i = 0; i < HEAP_CAPACITY; i++){
        if(fgets(line, sizeof(line), codes_in)!=0){
            char *p = line;
            in = *p;
            p++;
            p++;
            strcpy(code, p);
            //printf("lett: %c, code: %s", in, code);
            TNODE *curr = huffman_root;
            int len = strlen(code);
            for(int i = 0; i<(len-1); i++){
                int bit = code[i]-'0';
                //printf("%d\n", bit);
                if(bit == 1){
                    if(!curr->right){
                        curr->right = (TNODE *)malloc(sizeof(TNODE));
                        curr->right->type = INT;
                    }
                    curr = curr->right;
                } else {
                    if(!curr->left){
                        curr->left = (TNODE *)malloc(sizeof(TNODE));
                        curr->left->type = INT;
                    }
                    curr = curr->left;
                }
            }
            curr->val = in;
            curr->type = LEAF;
        }
    }
    fclose(codes_in);

    FILE *decoded = fopen("decoded.txt", "w");
    FILE *bin = fopen("compressed.bin", "rb");

    TNODE *curr = huffman_root;
    char c, bit;
    char mask = 1 << 7;
    int i;
    while(!feof(bin)){
        c = fgetc(bin);
        if(feof(bin)) break;
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