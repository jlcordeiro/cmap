#include <stdio.h>
#include <string.h>
#include <minunit/minunit.h>
#include "../map.h"

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
    struct map_t* test = new_map();

    mu_assert(map_get(test, name1) == NULL, "Should be NULL.");

    map_set(test, name1, (void*)value1);
    map_set(test, name1, (void*)value1);
    map_set(test, name1, (void*)value1);
    map_set(test, name1, (void*)value1);
    map_set(test, name1, (void*)value1);
    map_set(test, name1, (void*)value1);
    map_set(test, name1, (void*)value1);
    mu_check(strcmp((const char*)map_get(test, name1), value1) == 0);

    destroy_map(&test);
}

MU_TEST(replace_check)
{
    struct map_t* test = new_map();

    map_set(test, name1, (void*)value1);
    mu_check(strcmp((const char*)map_get(test, name1), value1) == 0);

    map_set(test, name1, (void*)value2);
    mu_check(strcmp((const char*)map_get(test, name1), value2) == 0);

    destroy_map(&test);
}

MU_TEST(multiple_check)
{
    struct map_t* test = new_map();

    map_set(test, name1, (void*)value1);
    mu_check(strcmp((const char*)map_get(test, name1), value1) == 0);

    map_set(test, name2, (void*)value2);
    mu_check(strcmp((const char*)map_get(test, name2), value2) == 0);

    map_set(test, name3, (void*)value3);
    mu_check(strcmp((const char*)map_get(test, name3), value3) == 0);

    destroy_map(&test);
}

MU_TEST(casesensitive_check)
{
    struct map_t* test = new_map();

    map_set(test, name1_lcase, (void*)value1);
    map_set(test, name1_ucase, (void*)value2);

    mu_check(strcmp((const char*)map_get(test, name1_lcase), value2) == 0);
    mu_check(strcmp((const char*)map_get(test, name1_ucase), value2) == 0);

    destroy_map(&test);
}

MU_TEST(del_first_check) {
    struct map_t* test = new_map();

    map_set(test, name1, (void*)value1);
    map_set(test, name2, (void*)value2);
    map_set(test, name3, (void*)value3);

    map_del(test, name1);

    mu_check(map_get(test, name1) == NULL);
    mu_check(strcmp((const char*)map_get(test, name2), value2) == 0);
    mu_check(strcmp((const char*)map_get(test, name3), value3) == 0);

    destroy_map(&test);
}

MU_TEST(del_middle_check) {
    struct map_t* test = new_map();

    map_set(test, name1, (void*)value1);
    map_set(test, name2, (void*)value2);
    map_set(test, name3, (void*)value3);

    map_del(test, name2);

    mu_check(strcmp((const char*)map_get(test, name1), value1) == 0);
    mu_check(map_get(test, name2) == NULL);
    mu_check(strcmp((const char*)map_get(test, name3), value3) == 0);

    destroy_map(&test);
}

MU_TEST(del_last_check) {
    struct map_t* test = new_map();

    map_set(test, name1, (void*)value1);
    map_set(test, name2, (void*)value2);
    map_set(test, name3, (void*)value3);

    map_del(test, name3);

    mu_check(strcmp((const char*)map_get(test, name1), value1) == 0);
    mu_check(strcmp((const char*)map_get(test, name2), value2) == 0);
    mu_check(map_get(test, name3) == NULL);

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

int main()
{
    MU_RUN_SUITE(main_suite);
    MU_RUN_SUITE(del_suite);
    MU_REPORT();
}
