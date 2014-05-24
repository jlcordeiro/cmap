//
// map.c
//
// Copyright (c) 2014 Joao Cordeiro
// MIT licensed

#ifdef __APPLE__
#include <stdlib.h>
#include <strings.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>
#include "map.h"

static struct map_node_t* new_map_node(const char* key, void* value)
{
    struct map_node_t* new_node = NULL;

    new_node = (struct map_node_t*)malloc(sizeof(struct map_node_t));

    if (new_node) {
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

static int compare_key(const char* key1, const char* key2, enum case_sensitivity_t cs)
{
    switch (cs) {
        case CASE_INSENSITIVE:
            return strcasecmp(key1, key2);
        case CASE_SENSITIVE:
            return strcmp(key1, key2);
        default:
            return strcmp(key1, key2);
    }
}

static struct map_node_t* map_find(struct map_t* map, const char* key)
{
    struct map_node_t* node;
    for (node = map->head; node != NULL; node = node->next) {
        if (!compare_key(key, node->key, map->case_s)) {
            return node;
        }
    }

    return node;
}

struct map_t* new_map(enum case_sensitivity_t cs)
{
    struct map_t* map = NULL;

    map = (struct map_t*)malloc(sizeof(struct map_t));

    if (map) {
        map->head = NULL;
        map->case_s = cs;
        map->size = 0;
        map->free_func = NULL;
    }

    return map;
}

void* map_get(struct map_t* map, const char* key)
{
    const struct map_node_t* node = map_find(map, key);

    if (node) {
        return node->value;
    }

    return NULL;
}

void map_set(struct map_t* map, const char* key, void* value)
{
    struct map_node_t* matching_node = map_find(map, key);

    if (matching_node) {
        if (map->free_func && matching_node->value) {
            (*map->free_func)(matching_node->value);
        }

        matching_node->value = value;
        return;
    }

    if (map->head == NULL) {
        map->head = new_map_node(key, value);
    } else {
        struct map_node_t* node;
        for (node = map->head; node->next != NULL; node = node->next) {
        }
        node->next = new_map_node(key, value);
        node->next->prev = node;
    }

    map->size++;
}

void map_del(struct map_t* map, const char* key)
{
    struct map_node_t* matching_node = map_find(map, key);

    if (!matching_node) {
        return;
    }

    if (map->free_func && matching_node->value) {
        (*map->free_func)(matching_node->value);
    }

    if (matching_node == map->head) {
        map->head = map->head->next;
    } else {
        matching_node->prev->next = matching_node->next;
    }

    free(matching_node->key);
    free(matching_node);

    map->size--;
}

void destroy_map(struct map_t** map)
{
    struct map_node_t* node;
    struct map_node_t* next_node;

    for (node = (*map)->head; node != NULL; node = next_node) {
        free(node->key);

        if ((*map)->free_func && node->value) {
            (*(*map)->free_func)(node->value);
        }

        next_node = node->next;
        free(node);
        node = NULL;
    }

    free(*map);
    *map = NULL;
}

size_t map_size(struct map_t* map)
{
    return map->size;
}

void map_set_free_func(struct map_t* map, void (*f)(void*)) {
    map->free_func = f;
}
