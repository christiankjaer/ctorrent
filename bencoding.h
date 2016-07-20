#ifndef BENCODING_H
#define BENCODING_H

#include <stdio.h>

enum BENC_TYPE { B_STRING, B_INT, B_LIST, B_DICT };

typedef struct dict_t dict_t;
typedef struct node_t node_t;
typedef struct bencoding_t bencoding_t;

struct node_t {
    bencoding_t* data;
    node_t* next;
};

struct dict_t {
    char* key;
    bencoding_t* data;
    dict_t* next;
};

typedef struct bencoding_t {
    int type;
    union {
        char* b_string;
        int b_int;
        node_t* b_list;
        dict_t* b_dict;
    } data;
} bencoding_t;

bencoding_t* read_bencoding(FILE*);
bencoding_t* read_string(FILE*);
bencoding_t* read_int(FILE*);
bencoding_t* read_list(FILE*);
bencoding_t* read_dict(FILE*);

bencoding_t* find_in_dict(bencoding_t*, char*);

void print_bencoding(bencoding_t*);

#endif
