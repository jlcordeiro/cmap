Small map implementation with char* keys and void* values.

Usage
=====

```
const char* name1 = "__one";
const char* name2 = "__two";
const char* value1 = "__1";
const char* value2 = "__2";

struct map_t* test = new_map(CASE_SENSITIVE);

map_set(test, name1, (void*)value1);
map_set(test, name2, (void*)value2);

printf("%s => %s\n", name2, map_get(test, name2));

map_del(test, name2);
map_del(test, name1);

destroy_map(&test);
```


Running tests
=============

Install dependencies
--------------------

To install the dependencies you will need clib and then run the command below:

```
make deps -C test
```

Compile and run
---------------
```
make tests
./test/map_test
```
