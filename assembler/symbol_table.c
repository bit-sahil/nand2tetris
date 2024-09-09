#include<stdio.h>
#include<stdlib.h>


/* A simple map with key value store. Instead of implementing hash functions, doing it with 
sequential logic for quicker implementation. Since we're talking about just a few items in map to begin with,
it's not too bad. And implementation could be improved later, as required.
In small machines, this might've been better implementation, considering resources.

Using map instead of symbol_table cos map is equally intuitive and less typing, avoids bulky names like new_symbol_table and struct SymbolTable
Also, I think reserving SymbolTable for map pre-populated with reserved keywords
*/


#define True 1
#define False 0


struct Node {
    char key[256];
    char value[16];
};


struct Map {
    int size;
    int cnt;
    struct Node *nodes;
};


struct Map* new_map(int size) {
    // Allocate space for a map with given size
    // return: address of newly created map

    struct Map* map = (struct Map*) malloc(sizeof(struct Map));
    map->size = size;
    map->cnt = 0;
    struct Node* nodes = (struct Node*) malloc(size * sizeof(struct Node));
    if(nodes == NULL) printf("Error allocating memory for nodes");
    map->nodes = nodes;
    return map;
}


void delete_map(struct Map* map) {
    //deallocate/free space allocated for given map
    free(map->nodes);
    free(map);
}


void print_line(char* line) {
    // just prints a line. %s was printing random characters so resorted to a basic function
    int idx = 0;
    while(1) {
        printf("%c", line[idx]);
        if((line[idx] == '\n') || (line[idx] == '\0')) {
            break;
        }
        idx++;
    }
}


int compare_str(char* s1, char* s2) {
    // compare string character by character
    int i =0;
    // printf("%s;%s\n", s1, s2);
    while(s1[i] == s2[i]) {
        if(s1[i] == '\0') return True;
        i++;
    }
    return False;
}


void copy_str(char* dest, char* src) {
    // assume destination has required space and copy source string characters
    // strcpy was possibly copying erronously and hence this function (at least printing is erronous)
    int i = 0;
    while(1) {
        dest[i] = src[i];
        if(src[i] == '\0') return;
        i++;
    }
}


void print_map(struct Map map) {
    // printf("Map count: %d\n", map.cnt);
    
    //printf("print_map; &map: %p; &nodes: %p; &nodes[0]: %p; \n", &map, &(map.nodes), &(map.nodes[0]));
    for(int i=0;i<map.cnt;i++) {
        printf("%s:%s\n", map.nodes[i].key, map.nodes[i].value);
    }
}


void add_key(struct Map* map, char* key, char* value) {
    if(map->cnt == map->size) { 
        printf("Exceeded number of elements in map \n");
        return;
    }
    
    //printf("&map: %p; &nodes: %p; &nodes[0]: %p; \n", map, (map->nodes), &(map->nodes[0])); 
    copy_str(map->nodes[map->cnt].key, key);
    copy_str(map->nodes[map->cnt].value, value);
    map->cnt = map->cnt + 1;
}


int get_value(struct Map* map, char* key, char* value) {
    //map and key are given arguments, result is stored in value
    //returns True if result is found in dict, else False
    //O(n) solution to being with, this function can be improved independently
    
    struct Node node;
    for(int i=0;i<map->cnt;i++) {
        node = map->nodes[i];
        if(compare_str(key, node.key)) {
            copy_str(value, node.value);
            return True;
        }
    }
    
    return False;
}


int main() {
    struct Map* map = new_map(5);
    char s1[] = "Hellow";
    char s2[] = "tableau";
    add_key(map, s1, s2);
    add_key(map, s2, s1);
    print_map(*map);
    char value[16];
    int v = get_value(map, s2, value);
    printf("v=%d\n", v);
    printf("value=%s\n", value);
    int v2 = get_value(map, "N", value);
    printf("v2=%d\n", v2);
    delete_map(map);
}

