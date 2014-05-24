// map.h

#ifndef __MAP_H__
#define __MAP_H__

#ifdef __cplusplus
extern "C" {
#endif

struct map_node_t {
    char* key;
    void* value;

    struct map_node_t* next;
    struct map_node_t* prev;
};

struct map_t {
    struct map_node_t* head;
};

struct map_t* new_map();
void destroy_map(struct map_t** map);

void* map_get_value(struct map_t* map, const char* key);
void map_set(struct map_t* map, const char* key, void* value);
void map_del(struct map_t* map, const char* key);


#ifdef __cplusplus
}
#endif

#endif

