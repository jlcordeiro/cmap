// map.h

#ifndef __MAP_H__
#define __MAP_H__

#ifdef __cplusplus
extern "C" {
#endif

enum case_sensitivity_t {
    CASE_SENSITIVE,
    CASE_INSENSITIVE
};

struct map_node_t {
    char* key;
    void* value;

    struct map_node_t* next;
    struct map_node_t* prev;
};

struct map_t {
    struct map_node_t* head;
    enum case_sensitivity_t case_s;
    size_t size;
};

struct map_t* new_map(enum case_sensitivity_t cs);
void destroy_map(struct map_t** map);

void* map_get(struct map_t* map, const char* key);
void map_set(struct map_t* map, const char* key, void* value);
void map_del(struct map_t* map, const char* key);

size_t map_size(struct map_t* map);

#ifdef __cplusplus
}
#endif

#endif

