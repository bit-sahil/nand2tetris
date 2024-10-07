
#ifndef ABSTRACT_MAP_H
#define ABSTRACT_MAP_H


typedef struct AbstractNode {
    char* key;
    void* value; // any struct/type whatsoever
} AbstractNode;


typedef struct AbstractMap {
    int size;
    int cnt;
    struct AbstractNode *nodes;

    int key_len;
} AbstractMap;


AbstractMap* init_abstract_map(int size, int key_len);

void dealloc_map(AbstractMap* map);

void print_abstract_map(AbstractMap* map, void (*print_val)(void*) );

void add_abstract_key(AbstractMap* map, char* key, void* value);

void* get_abstract_value(AbstractMap* map, char* key);

#endif
