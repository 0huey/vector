# My C vector implementation

## Usage

The vector object contains members that should only be modified by the vector API.

```C
typedef struct vector
{
    void* array;
    size_t size;
    size_t max_size;
    size_t element_size;
} vector;
```

### Using vector

```C
// declare a vector object
vector vec;

// then initialize it with the size of the object you will be storing
vector_init(&vec, sizeof(int));

// then append some elements by passing a pointer to the data
for (int i = 0; i < 10; i++)
{
    vector_append(&vec, &i)
}

// then access the elements by casting vector.array to a pointer of your type
int* int_array = (int*)vec.array;

// use vector.size for bounds checking
for (int i = 0; i < vec.size; i++)
{
    do_something(int_array[i]);
}

// you must always recast the array pointer after appending to the vector
// it may have been resized, and your pointer will be dangling
int num = 99;
vector_append(&vec, &num);
do_something(int_array); // undefined

// when you're finished with the vector, release it
vector_destroy(&vec);

// vector_destroy only releases the internal resources
// it does not know if you malloc'd the vector object itself
vector* vec2 = malloc(sizeof(vector)); // must be freed
vector_init(vec2, sizeof(int));
vector_destroy(vec2);
free(vec2);
```
