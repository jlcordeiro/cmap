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

MU_TEST(null_check)
{
    struct map_t* test = new_map(CASE_SENSITIVE);

    mu_assert(map_get(test, name1) == NULL, "Should be NULL.");
    mu_assert(map_size(test) == 0, "Wrong size");

    map_set(test, name1, (void*)value1);
    mu_check(strcmp((const char*)map_get(test, name1), value1) == 0);
    mu_assert(map_size(test) == 1, "Wrong size");

    destroy_map(&test);
}

MU_TEST(replace_check)
{
    struct map_t* test = new_map(CASE_SENSITIVE);

    map_set(test, name1, (void*)value1);
    mu_check(strcmp((const char*)map_get(test, name1), value1) == 0);
    mu_assert(map_size(test) == 1, "Wrong size");

    map_set(test, name1, (void*)value2);
    mu_check(strcmp((const char*)map_get(test, name1), value2) == 0);
    mu_assert(map_size(test) == 1, "Wrong size");

    destroy_map(&test);
}

MU_TEST(multiple_check)
{
    struct map_t* test = new_map(CASE_SENSITIVE);

    map_set(test, name1, (void*)value1);
    mu_check(strcmp((const char*)map_get(test, name1), value1) == 0);
    mu_assert(map_size(test) == 1, "Wrong size");

    map_set(test, name2, (void*)value2);
    mu_check(strcmp((const char*)map_get(test, name2), value2) == 0);
    mu_assert(map_size(test) == 2, "Wrong size");

    map_set(test, name3, (void*)value3);
    mu_check(strcmp((const char*)map_get(test, name3), value3) == 0);
    mu_assert(map_size(test) == 3, "Wrong size");

    destroy_map(&test);
}

MU_TEST(casesensitive_check)
{
    struct map_t* test = new_map(CASE_SENSITIVE);

    map_set(test, name1_lcase, (void*)value1);
    map_set(test, name1_ucase, (void*)value2);

    mu_check(strcmp((const char*)map_get(test, name1_lcase), value1) == 0);
    mu_check(strcmp((const char*)map_get(test, name1_ucase), value2) == 0);
    mu_assert(map_size(test) == 2, "Wrong size");

    destroy_map(&test);

    test = new_map(CASE_INSENSITIVE);

    map_set(test, name1_lcase, (void*)value1);
    map_set(test, name1_ucase, (void*)value2);

    mu_check(strcmp((const char*)map_get(test, name1_lcase), value2) == 0);
    mu_check(strcmp((const char*)map_get(test, name1_ucase), value2) == 0);
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
    mu_check(strcmp((const char*)map_get(test, name2), value2) == 0);
    mu_check(strcmp((const char*)map_get(test, name3), value3) == 0);
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

    mu_check(strcmp((const char*)map_get(test, name1), value1) == 0);
    mu_check(map_get(test, name2) == NULL);
    mu_check(strcmp((const char*)map_get(test, name3), value3) == 0);
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

    mu_check(strcmp((const char*)map_get(test, name1), value1) == 0);
    mu_check(strcmp((const char*)map_get(test, name2), value2) == 0);
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

    struct person_t {
        char* name;
    };

    struct person_t* person1 = (struct person_t*)malloc(sizeof(struct person_t));
    struct person_t* person2 = (struct person_t*)malloc(sizeof(struct person_t));
    struct person_t* person3 = (struct person_t*)malloc(sizeof(struct person_t));

    person1->name = (char*)malloc(strlen(value1)+1);
    person2->name = (char*)malloc(strlen(value2)+1);
    person3->name = (char*)malloc(strlen(value3)+1);

    strcpy(person1->name, value1);
    strcpy(person2->name, value2);
    strcpy(person3->name, value3);

    map_set(test, name1, (void*)person1);
    map_set(test, name2, (void*)person2);
    map_set(test, name3, (void*)person3);

    mu_assert(map_size(test) == 3, "Wrong size");

    void* v1 = map_get(test, name1);
    void* v2 = map_get(test, name2);
    void* v3 = map_get(test, name3);

    map_del(test, name1);
    map_del(test, name2);
    map_del(test, name3);

    free(person1);
    free(person1->name);
    free(person2);
    free(person2->name);
    free(person3);
    free(person3->name);

    mu_assert(map_size(test) == 0, "Wrong size");

    destroy_map(&test);
}

MU_TEST_SUITE(memcheck_suite) {
    MU_RUN_TEST(free_check);
}

int main()
{
    MU_RUN_SUITE(main_suite);
    MU_RUN_SUITE(del_suite);
    MU_RUN_SUITE(memcheck_suite);
    MU_REPORT();
}
