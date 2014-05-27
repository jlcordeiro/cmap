//
// map_test.c
//
// Copyright (c) 2014 Joao Cordeiro
// MIT licensed


#include <stdio.h>
#include <minunit/minunit.h>
#include "../map.h"
#ifdef __APPLE__
#include <stdlib.h>
#include <strings.h>
#else
#include <malloc.h>
#endif

const char* name1 = "__one";
const char* name2 = "__two";
const char* name3 = "__three";
const char* value1 = "__1";
const char* value2 = "__2";
const char* value3 = "__3";

const char* name1_lcase = "__one";
const char* name1_ucase = "__One";

struct person_t {
    char* name;
};

void free_person(void* vperson) {
    if (!vperson) {
        return;
    }

    struct person_t* person = (struct person_t*)vperson;

    if (person->name) {
        free(person->name);
    }

    free(person);
}

void _check_value(struct map_t* map, const char* key, const char* val) {
    const char* real_val = (const char*)map_get(map, key);
    mu_assert(strcmp(real_val, val) == 0, real_val);
}

MU_TEST(null_check)
{
    struct map_t* test = new_map(CASE_SENSITIVE);

    mu_assert(map_get(test, name1) == NULL, "Should be NULL.");
    mu_assert(map_size(test) == 0, "Wrong size");

    map_set(test, name1, (void*)value1);
    _check_value(test, name1, value1);
    mu_assert(map_size(test) == 1, "Wrong size");

    destroy_map(&test);
}

MU_TEST(replace_check)
{
    struct map_t* test = new_map(CASE_SENSITIVE);

    map_set(test, name1, (void*)value1);
    _check_value(test, name1, value1);
    mu_assert(map_size(test) == 1, "Wrong size");

    map_set(test, name1, (void*)value2);
    _check_value(test, name1, value2);
    mu_assert(map_size(test) == 1, "Wrong size");

    destroy_map(&test);
}

MU_TEST(multiple_check)
{
    struct map_t* test = new_map(CASE_SENSITIVE);

    map_set(test, name1, (void*)value1);
    _check_value(test, name1, value1);
    mu_assert(map_size(test) == 1, "Wrong size");

    map_set(test, name2, (void*)value2);
    _check_value(test, name2, value2);
    mu_assert(map_size(test) == 2, "Wrong size");

    map_set(test, name3, (void*)value3);
    _check_value(test, name3, value3);
    mu_assert(map_size(test) == 3, "Wrong size");

    destroy_map(&test);
}

MU_TEST(casesensitive_check)
{
    struct map_t* test = new_map(CASE_SENSITIVE);

    map_set(test, name1_lcase, (void*)value1);
    map_set(test, name1_ucase, (void*)value2);

    _check_value(test, name1_lcase, value1);
    _check_value(test, name1_ucase, value2);
    mu_assert(map_size(test) == 2, "Wrong size");

    destroy_map(&test);

    test = new_map(CASE_INSENSITIVE);

    map_set(test, name1_lcase, (void*)value1);
    map_set(test, name1_ucase, (void*)value2);

    _check_value(test, name1_lcase, value2);
    _check_value(test, name1_ucase, value2);
    mu_assert(map_size(test) == 1, "Wrong size");

    destroy_map(&test);
}

MU_TEST(del_first_check) {
    struct map_t* test = new_map(CASE_SENSITIVE);

    map_set(test, name1, (void*)value1);
    map_set(test, name2, (void*)value2);
    map_set(test, name3, (void*)value3);

    mu_assert(map_size(test) == 3, "Wrong size");

    map_del(test, name1);

    mu_check(map_get(test, name1) == NULL);
    _check_value(test, name2, value2);
    _check_value(test, name3, value3);
    mu_assert(map_size(test) == 2, "Wrong size");

    destroy_map(&test);
}

MU_TEST(del_middle_check) {
    struct map_t* test = new_map(CASE_SENSITIVE);

    map_set(test, name1, (void*)value1);
    map_set(test, name2, (void*)value2);
    map_set(test, name3, (void*)value3);

    mu_assert(map_size(test) == 3, "Wrong size");

    map_del(test, name2);

    _check_value(test, name1, value1);
    mu_check(map_get(test, name2) == NULL);

    _check_value(test, name3, value3);
    mu_assert(map_size(test) == 2, "Wrong size");

    destroy_map(&test);
}

MU_TEST(del_last_check) {
    struct map_t* test = new_map(CASE_SENSITIVE);

    map_set(test, name1, (void*)value1);
    map_set(test, name2, (void*)value2);
    map_set(test, name3, (void*)value3);

    mu_assert(map_size(test) == 3, "Wrong size");

    map_del(test, name3);

    _check_value(test, name1, value1);
    _check_value(test, name2, value2);
    mu_check(map_get(test, name3) == NULL);
    mu_assert(map_size(test) == 2, "Wrong size");

    destroy_map(&test);
}


MU_TEST_SUITE(main_suite) {
    MU_RUN_TEST(null_check);
    MU_RUN_TEST(replace_check);
    MU_RUN_TEST(multiple_check);
    MU_RUN_TEST(casesensitive_check);
}

MU_TEST_SUITE(del_suite) {
    MU_RUN_TEST(del_first_check);
    MU_RUN_TEST(del_middle_check);
    MU_RUN_TEST(del_last_check);
}

MU_TEST(free_check) {
    struct map_t* test = new_map(CASE_SENSITIVE);

    struct person_t* person1 = (struct person_t*)malloc(sizeof(struct person_t));
    struct person_t* person2 = (struct person_t*)malloc(sizeof(struct person_t));

    person1->name = (char*)malloc(strlen(value1)+1);
    person2->name = (char*)malloc(strlen(value2)+1);

    strcpy(person1->name, value1);
    strcpy(person2->name, value2);

    map_set(test, name1, (void*)person1);
    map_set(test, name2, (void*)person2);

    mu_assert(map_size(test) == 2, "Wrong size");

    void* v1 = map_get(test, name1);
    void* v2 = map_get(test, name2);

    map_del(test, name1);
    map_del(test, name2);

    free_person(person1);
    free_person(person2);

    mu_assert(map_size(test) == 0, "Wrong size");

    destroy_map(&test);
}

MU_TEST(destroy_check) {
    struct map_t* test = new_map(CASE_SENSITIVE);
    map_set_free_func(test, free_person);

    struct person_t* person1 = (struct person_t*)malloc(sizeof(struct person_t));
    struct person_t* person2 = (struct person_t*)malloc(sizeof(struct person_t));

    person1->name = (char*)malloc(strlen(value1)+1);
    person2->name = (char*)malloc(strlen(value2)+1);

    strcpy(person1->name, value1);
    strcpy(person2->name, value2);

    map_set(test, name1, (void*)person1);
    map_set(test, name2, (void*)person2);

    destroy_map(&test);
}

MU_TEST_SUITE(memcheck_suite) {
    MU_RUN_TEST(free_check);
    MU_RUN_TEST(destroy_check);
}

MU_TEST(sort_add_check) {
    struct map_t* map = new_map(CASE_SENSITIVE);

    map_set(map, "whatever", (void*)value1);
    map_set(map, "bla", (void*)value2);
    map_set(map, "edag", (void*)value2);
    map_set(map, "whatever2", (void*)value2);
    map_set(map, "bleu", (void*)value2);
    map_set(map, "std::", (void*)value2);

    struct map_node_t* node = map->head;
    mu_assert(strcmp(node->key, "bla") == 0, node->key);
    node = node->next;
    mu_assert(strcmp(node->key, "bleu") == 0, node->key);
    node = node->next;
    mu_assert(strcmp(node->key, "edag") == 0, node->key);
    node = node->next;
    mu_assert(strcmp(node->key, "std::") == 0, node->key);
    node = node->next;
    mu_assert(strcmp(node->key, "whatever") == 0, node->key);
    node = node->next;
    mu_assert(strcmp(node->key, "whatever2") == 0, node->key);

    destroy_map(&map);
}

MU_TEST_SUITE(order_suite) {
    MU_RUN_TEST(sort_add_check);
}

int main()
{
    MU_RUN_SUITE(main_suite);
    MU_RUN_SUITE(del_suite);
    MU_RUN_SUITE(memcheck_suite);
    MU_RUN_SUITE(order_suite);
    MU_REPORT();
}
