#include <stdlib.h>
#include <string.h>

#include "bencoding.h"

bencoding_t* read_bencoding(FILE* stream) {
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
    fscanf(stream, "%d:", &size);
    char* data = (char*)malloc(sizeof(char) * (size+1));
    fread(data, sizeof(char), size, stream);
    data[size] = '\0';
    bencoding_t* ret = (bencoding_t*)malloc(sizeof(bencoding_t));
    ret->type = B_STRING;
    ret->data.b_string = data;
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
    while ((c = fgetc(stream)) != 'e') {
        ungetc(c, stream);
        bencoding_t* data = read_bencoding(stream);
        node_t* elem = (node_t*)malloc(sizeof(node_t));
        elem->data = data;
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
    return NULL;
}

void print_string(bencoding_t* b) {
    printf("\"%s\"\n", b->data.b_string);
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
