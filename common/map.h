
#ifndef MAP_H
#define MAP_H

#include "abstract_map.h"


typedef AbstractMap Map;


Map* init_map(int size);

void print_map(Map* map);

void add_key(Map* map, char* key, char* value);

int get_value(Map* map, char* key, char* value);

#endif

