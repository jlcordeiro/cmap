//
// map.h
//
// Copyright (c) 2014 Joao Cordeiro
// MIT licensed

#ifndef __MAP_H__
#define __MAP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

//

enum case_sensitivity_t {
    CASE_SENSITIVE,
    CASE_INSENSITIVE
};

/**
 * \brief Node structure.
 */
struct map_node_t {
    char* key;
    void* value;

    struct map_node_t* next;
};

/**
 * \brief Map structure.
 */
struct map_t {
    /** Head node */
    struct map_node_t* head;
    /** Which case sensitivity for key comparison. */
    enum case_sensitivity_t case_s;

    /** Number of elements in the map. */
    size_t size;

    /** Function to use when deleting map values. */
    void (*free_func)(void*);
};


// Create and delete map functions

/**
 * \brief Create a new map with the chosen case sensitivity.
 * \param cs Case sensitivity to use in key comparison.
 * \return New map or NULL on failure.
 */
struct map_t* new_map(enum case_sensitivity_t cs);

/**
 * \brief Save the pointer to the function to use to free map values.
 * \param f Pointer to the function that is to be called on value deletion.
 */
void map_set_free_func(struct map_t* map, void (*f)(void*));

/**
 * \brief Destroy map. Free all memory.
 */
void destroy_map(struct map_t** map);


// Map access functions.

/**
 * \brief Get the value that corresponds to a key.
 * \return Value associated with the key. NULL if not found.
 */
void* map_get(struct map_t* map, const char* key);

/**
 * \brief Set a value in a map.
 * Adds a new entry if the key does not exist,
 * or replaces the value if it exists.
 * \return 0 if successful. -1 if it fails.
 */
int map_set(struct map_t* map, const char* key, void* value);

/**
 * \brief Remove an entry from the map.
 */
void map_del(struct map_t* map, const char* key);


// Utility functions

/**
 * \brief Get number of elements in a map.
 * \param map Map to be analysed.
 * \return Number of elements in a map.
 */
size_t map_size(struct map_t* map);

#ifdef __cplusplus
}
#endif

#endif

