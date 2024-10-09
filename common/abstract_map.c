#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>
#include "abstract_map.h"

/* A simple map with key value store. Instead of implementing hash functions, doing it with 
sequential logic for quicker implementation. Since we're talking about just a few items in map to begin with,
it's not too bad. And implementation could be improved later, as required.
In small machines, this might've been better implementation, considering resources.

*/


AbstractMap* init_abstract_map(int size, int key_len) {
    /* Allocate space for a map with given size and key_len
    returns address of newly created map
    */

    AbstractMap* map = (AbstractMap*) malloc(sizeof(AbstractMap));

    map->size = size;
    map->cnt = 0;
    map->key_len = key_len;

    map->nodes = (AbstractNode*) malloc(size * sizeof(AbstractNode));

    if(map->nodes == NULL) 
        printf("Error allocating memory for nodes");

    return map;
}


void dealloc_map(AbstractMap* map) {
    // deallocate/free space allocated for given map

    // space for key and value is allocated during add_key
    for(int i=0;i<map->cnt;i++) {
        free(map->nodes[i].key);
        free(map->nodes[i].value);
    }

    free(map->nodes);
    free(map);
}


void print_abstract_map(AbstractMap* map, void (*print_val)(void*) ) {
    /* Print map sequentially, taking in a print_val function which prints void* value
    */
    
    for(int i=0;i<map->cnt;i++) {
        printf("key=%s; ", map->nodes[i].key);
        print_val(map->nodes[i].value);
        printf("\n");
    }
}


AbstractNode* find_node(AbstractMap* map, char* key) {
    /* map and key are given arguments, return node if found
    else return NULL

    O(n) solution to being with, this function can be improved independently
    */
    
    AbstractNode node;
    for(int i=0;i<map->cnt;i++) {
        node = map->nodes[i];
        if(strcmp(key, node.key) == 0) {
            return &map->nodes[i];
        }
    }
    
    return NULL;
}


int update_abstract_key(AbstractMap* map, char* key, void* value) {
    /* update value in given map if key is found, and return true
    if key not find, return false

    */

    AbstractNode* node = find_node(map, key);

    if(node==NULL)
        return false;

    node->value = value;
    return true;
}


void add_abstract_key(AbstractMap* map, char* key, void* value) {
    /* add key-value pair to given map
    allocate memory to key variable of AbstractNode, and copy key into it
    value variable of AbstractNode points to value

    */

    if(map->cnt == map->size) { 
        printf("Exceeded number of elements in map \n");
        return;
    }

    //printf("&map: %p; &nodes: %p; &nodes[0]: %p; \n", map, (map->nodes), &(map->nodes[0])); 

    map->nodes[map->cnt].key = (char*) malloc(map->key_len * sizeof(char));
    strcpy(map->nodes[map->cnt].key, key);
    
    map->nodes[map->cnt].value = value;

    map->cnt = map->cnt + 1;
}


void add_or_update_abstract_key(AbstractMap* map, char* key, void* value) {
    // try update, if key is present
    // if not present, add new key

    if(update_abstract_key(map, key, value))
        return;

    add_abstract_key(map, key, value);
}


void* get_abstract_value(AbstractMap* map, char* key) {
    /* map and key are given arguments, result is returned as a pointer to value
    returns NULL if value is not found

    O(n) solution to being with, this function can be improved independently
    */
    
    AbstractNode* node = find_node(map, key);

    if(node==NULL)
        return NULL;
    
    return node->value;
}
