//
// map.h
//

#ifdef __APPLE__
#include <stdlib.h>
#include <strings.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>
#include <string.h>
#include "map.h"

static struct map_node_t* new_map_node(const char* key, void* value)
{
    struct map_node_t* new_node = NULL;
    
    new_node = (struct map_node_t*)malloc(sizeof(struct map_node_t));

    if (new_node)
    {
        new_node->next = NULL;
        new_node->prev = NULL;
        new_node->value = value;

        new_node->key = (char*)malloc(strlen(key) + 1);
        if (!new_node->key) {
            free(new_node);
            return NULL;
        }

        strcpy(new_node->key, key);
    }

    return new_node;
}

static struct map_node_t* map_find(struct map_t* map, const char* key)
{
    struct map_node_t* node;
    for (node = map->head; node != NULL; node = node->next) {
        if (!strcasecmp(key, node->key)) {
            return node;
        }
    }

    return node;
}

struct map_t* new_map()
{
    struct map_t* map = NULL;
    
    map = (struct map_t*)malloc(sizeof(struct map_t));

    if (map)
    {
        map->head = NULL;
    }

    return map;
}

void* map_get(struct map_t* map, const char* key)
{
    const struct map_node_t* node = map_find(map, key);

    if (node)
    {
        return node->value;
    }

    return NULL;
}

void map_set(struct map_t* map, const char* key, void* value)
{
    struct map_node_t* matching_node = map_find(map, key);

    if (matching_node)
    {
        matching_node->value = value;
        return;
    }

    if (map->head == NULL)
    {
        map->head = new_map_node(key, value);
    }
    else
    {
        struct map_node_t* node;
        for (node = map->head; node->next != NULL; node = node->next) {
        }
        node->next = new_map_node(key, value);
        node->next->prev = node;
    }
}

void map_del(struct map_t* map, const char* key)
{
    struct map_node_t* matching_node = map_find(map, key);

    if (!matching_node)
    {
        return;
    }

    if (matching_node == map->head)
    {
        map->head = map->head->next;
    }
    else
    {
        matching_node->prev->next = matching_node->next;
    }

    free(matching_node->key);
    free(matching_node);
}

void destroy_map(struct map_t** map)
{
    struct map_node_t* node;
    struct map_node_t* next_node;

    for (node = (*map)->head; node != NULL; node = next_node) {
        free(node->key);

        next_node = node->next;
        free(node);
        node = NULL;
    }

    free(*map);
    *map = NULL;
}
