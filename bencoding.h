#ifndef BENCODING_H
#define BENCODING_H

#include <stdio.h>

enum BENC_TYPE { B_STRING, B_INT, B_LIST, B_DICT };

/* To be able to refer mutually to other structs
 * lists contain bencoding_t and vice versa.
 */
typedef struct dict_t dict_t;
typedef struct node_t node_t;
typedef struct bencoding_t bencoding_t;


/* Linked list of bencoding structs */
struct node_t {
    bencoding_t* data;
    node_t* next;
};

/* Stupid dict implementation as an associativity list
 * Every link of the list contains the key and the value
 */
struct dict_t {
    char* key;
    bencoding_t* data;
    dict_t* next;
};

typedef struct bencoding_t {
    int type; /* BENC_TYPE */
    union {
        /* We represent strings as char array and size
         * and not as zero terminated strings, because
         * they can include binary data as well
         */
        struct b_string {
            int size;
            char* buffer;
        } b_string;
        int b_int;
        /* List type */
        node_t* b_list;
        /* Dict type */
        dict_t* b_dict;
    } data;
} bencoding_t;

bencoding_t* read_bencoding(FILE*);
bencoding_t* read_string(FILE*);
bencoding_t* read_int(FILE*);
bencoding_t* read_list(FILE*);
bencoding_t* read_dict(FILE*);

bencoding_t* find_in_dict(bencoding_t*, char*);
int get_pieces(bencoding_t*, char**);

void print_bencoding(bencoding_t*);

void destroy_list(node_t*);
void destroy_dict(dict_t*);
void destroy_bencoding(bencoding_t*);

#endif
