
#ifndef MAP_H
#define MAP_H


struct Node {
    char key[256];
    char value[17];  // 16-bit for address and one bit for string termination ('\0')
};


struct Map {
    int size;
    int cnt;
    struct Node *nodes;
};


struct Map* new_map(int size);

void delete_map(struct Map* map);

void print_map(struct Map map);

void add_key(struct Map* map, char* key, char* value);

int get_value(struct Map* map, char* key, char* value);

#endif

