#include <stdlib.h>
#include <string.h>

#include "bencoding.h"

/* The format is unambiguous, and we can decide
 * whatever the type is by reading the next
 * character
 */
bencoding_t* read_bencoding(FILE* stream) {
    /* Get one character from the input */
    char c = fgetc(stream);
    switch (c) {
        case 'i': 
            return read_int(stream);
        case 'l':
            return read_list(stream);
        case 'd':
            return read_dict(stream);
        default:
            ungetc(c, stream);
            return read_string(stream);
    }
}
bencoding_t* read_string(FILE* stream) {
    int size;
    /* First element is the length of the string */
    fscanf(stream, "%d:", &size);
    /* We allocate an array of that size */
    char* data = (char*)malloc(sizeof(char) * size);
    /* We then read that amount of chars from the
     * input stream */
    fread(data, sizeof(char), size, stream);
    /* We allocate a bencoding struct */
    bencoding_t* ret = (bencoding_t*)malloc(sizeof(bencoding_t));
    /* And sets the properties */
    ret->type = B_STRING;
    ret->data.b_string.buffer = data;
    ret->data.b_string.size = size;
    return ret;
}
bencoding_t* read_int(FILE* stream) {
    int i;
    fscanf(stream, "%de", &i);
    bencoding_t* ret = (bencoding_t*)malloc(sizeof(bencoding_t));
    ret->type = B_INT;
    ret->data.b_int = i;
    return ret;
}
bencoding_t* read_list(FILE* stream) {
    bencoding_t* ret = (bencoding_t*)malloc(sizeof(bencoding_t));
    ret->type = B_LIST;
    ret->data.b_list = NULL;
    node_t** pp = &ret->data.b_list;
    char c;
    /* We read data until we reach an 'e', which
     * is the end of the list */
    while ((c = fgetc(stream)) != 'e') {
        ungetc(c, stream);
        /* read the element in the list recursively */
        bencoding_t* data = read_bencoding(stream);
        /* Allocate the new element in the list */
        node_t* elem = (node_t*)malloc(sizeof(node_t));
        elem->data = data;
        /* Pointer magic */
        elem->next = NULL;
        *pp = elem;
        pp = &elem->next;
    }
    return ret;
}
bencoding_t* read_dict(FILE* stream) {
    bencoding_t* ret = (bencoding_t*)malloc(sizeof(bencoding_t));
    ret->type = B_DICT;
    ret->data.b_dict = NULL;
    dict_t** pp = &ret->data.b_dict;
    char c;
    while ((c = fgetc(stream)) != 'e') {
        ungetc(c, stream);
        int size;
        fscanf(stream, "%d:", &size);
        char* key = (char*)malloc(sizeof(char) * (size+1));
        fread(key, sizeof(char), size, stream);
        key[size] = '\0';
        bencoding_t* data = read_bencoding(stream);
        dict_t* elem = (dict_t*)malloc(sizeof(dict_t));
        elem->key = key;
        elem->data = data;
        elem->next = NULL;
        *pp = elem;
        pp = &elem->next;
    }
    return ret;
}

/* Looks up a value in a dict.
 * Returns NULL if b is not a dict
 * Returns b if the key is not found
 */
bencoding_t* find_in_dict(bencoding_t* b, char* key) {
    if (b->type != B_DICT) {
        return NULL;
    }
    dict_t* p = b->data.b_dict;
    while (p) {
        if (strcmp(key, p->key) == 0) {
            return p->data;
        }
        p = p->next;
    }
    return p->data;
}

int get_pieces(bencoding_t* b, char** p) {
    bencoding_t* ben = find_in_dict(find_in_dict(b, "info"), "pieces");
    *p = ben->data.b_string.buffer;
    return ben->data.b_string.size;
}

void print_string(bencoding_t* b) {
    putchar('\"');
    for (int i = 0; i < b->data.b_string.size; ++i) {
        putchar(b->data.b_string.buffer[i]);
    }
    putchar('\"');
}
void print_int(bencoding_t* b) {
    printf("%d\n", b->data.b_int);
}
void print_list(bencoding_t* b) {
    node_t* p = b->data.b_list;
    puts("[");
    while (p) {
        print_bencoding(p->data);
        p = p->next;
    }
    puts("]");
}

void print_dict(bencoding_t* b) {
    dict_t* p = b->data.b_dict;
    puts("{");
    while (p) {
        printf("%s: ", p->key);
        print_bencoding(p->data);
        p = p->next;
    }
    puts("}");
}

void print_bencoding(bencoding_t* b) {
    switch (b->type) {
        case B_INT:
            print_int(b);
            break;
        case B_STRING:
            print_string(b);
            break;
        case B_LIST:
            print_list(b);
            break;
        default:
            print_dict(b);
    }
}
