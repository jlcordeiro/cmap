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

This implementation allows the developer to provide custom deallocation functions, so that all memory is properly cleared on deletion/destruction.

```
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

int main() {
    const char* p1 = "p1";
    const char* p2 = "p2";
    const char* name1 = "john";
    const char* name2 = "doe";

    struct map_t* person_map = new_map(CASE_SENSITIVE);

    map_set_free_func(person_map, free_person); // <-- tell the map how to deallocate each person

    struct person_t* person1 = (struct person_t*)malloc(sizeof(struct person_t));
    struct person_t* person2 = (struct person_t*)malloc(sizeof(struct person_t));

    person1->name = (char*)malloc(strlen(name1)+1);
    person2->name = (char*)malloc(strlen(name2)+1);

    strcpy(person1->name, name1);
    strcpy(person2->name, name2);

    map_set(person_map, p1, (void*)person1);
    map_set(person_map, p2, (void*)person2);

    destroy_map(&person_map);

    return 0;
}
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
