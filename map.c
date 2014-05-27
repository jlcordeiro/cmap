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

// Utility functions

/**
 * \brief Create a node.
 * \param key Entry key.
 * \param value Entry value.
 * \return A new node object. In case of error, returns NULL.
 */
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

/**
 * \brief Compare two keys, according to the chosen case sensitivity.
 * \param key1 First key.
 * \param key2 Second key.
 * \param cs Case sensitivity.
 * \return strcmp or strcasecmp result, depending on cs.
 */
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

/**
 * \brief Find a node with a given key.
 * \param map Map to search for the key in.
 * \param key Key to search for.
 * \return Pointer to node if found. NULL otherwise.
 */
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


// Lib functions

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

void map_set_free_func(struct map_t* map, void (*f)(void*))
{
    map->free_func = f;
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


void* map_get(struct map_t* map, const char* key)
{
    const struct map_node_t* node = map_find(map, key);

    if (node) {
        return node->value;
    }

    return NULL;
}

int map_set(struct map_t* map, const char* key, void* value)
{
    // empty list
    if (map->head == NULL) {
        map->head = new_map_node(key, value);

        if (!map->head) {
            return -1;
        }

        map->size = 1;
        return 0;
    }

    // create the new node.
    struct map_node_t* new_node = new_map_node(key, value);

    if (!new_node) {
        return -1;
    }

    // put the new node in the proper place
    struct map_node_t* node = NULL;
    struct map_node_t* prev_node = node;
    for (node = map->head; node; node = node->next) {
        int cmp = compare_key(key, node->key, map->case_s);

        // found node with same key
        if (cmp == 0) {
            if (map->free_func && node->value) {
                (*map->free_func)(node->value);
            }

            node->value = value;
            return 0;
        }

        // found node with a key with greater value
        if (cmp < 0) {
            break;
        }

        prev_node = node;
    }

    new_node->prev = prev_node;
    new_node->next = node;

    if (node) {
        node->prev = new_node;
    }

    if (prev_node) {
        prev_node->next = new_node;
    } else {
        map->head = new_node;
    }

    map->size++;
    return 0;
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


size_t map_size(struct map_t* map)
{
    return map->size;
}
