#include <vector.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#ifdef NDEBUG
#undef NDEBUG
#endif

extern const size_t VECTOR_DEFAULT_SIZE;
extern const double VECTOR_RESIZE_FACTOR;

struct test_struct
{
    char str[16];
    size_t num;
};

void test_vector_init(void);
void test_vector_destroy(void);
void test_vector_append(void);
void test_vector_clear(void);
void test_vector_resize(void);
void test_vector_copy(void);
void test_vector_pop(void);
void test_vector_remove(void);
void test_vector_remove_swapback(void);

int main(void)
{
    puts("[*] testing vector");
    printf("[*] vector initial size: %lu; resize factor: %f\n", VECTOR_DEFAULT_SIZE, VECTOR_RESIZE_FACTOR);

    test_vector_init();
    test_vector_destroy();
    test_vector_append();
    test_vector_clear();
    test_vector_resize();
    test_vector_copy();
    test_vector_pop();
    test_vector_remove();
    test_vector_remove_swapback();

    puts("[*] all vector tests pass");

    return 0;
}

void test_vector_init(void)
{
    vector vec;
    memset(&vec, 0, sizeof(vec));

    assert(vector_init(NULL, 1) == VECTOR_INVALID_PARAM);
    assert(vector_init(&vec, 0) == VECTOR_INVALID_PARAM);
    assert(vector_init(&vec, -1) == VECTOR_ALLOC_ERROR);

    vec.size = 9999;

    assert(vector_init(&vec, sizeof(size_t)) == VECTOR_SUCCESS);
    assert(vec.array != NULL);
    assert(vec.size == 0);
    assert(vec.element_size == sizeof(size_t));
    assert(vec.max_size == VECTOR_DEFAULT_SIZE);

    vector_destroy(&vec);

    assert(vector_init_with_size(&vec, sizeof(int), VECTOR_DEFAULT_SIZE * 2) == VECTOR_SUCCESS);
    assert(vec.max_size == VECTOR_DEFAULT_SIZE * 2);
    vector_destroy(&vec);

    puts("[+] vector_init pass");
}

void test_vector_destroy(void)
{
    vector vec;
    vector_init(&vec, sizeof(size_t));
    vector_destroy(&vec);

    vector nullvec;
    memset(&nullvec, 0, sizeof(vector));

    assert(memcmp(&vec, &nullvec, sizeof(vector)) == 0);
    puts("[+] vector_destroy pass");
}

void test_vector_append(void)
{
    vector vec;
    vector_init(&vec, sizeof(size_t));

    size_t num_elements = VECTOR_DEFAULT_SIZE * 10;

    for (size_t i = 0; i < num_elements; i++)
    {
        vector_append(&vec, &i);
    }

    assert(vec.size == num_elements);

    size_t* array = (size_t*)vec.array;

    for (size_t i = 0; i < vec.size; i++)
    {
        assert(array[i] == i);
    }

    vector_destroy(&vec);
    puts("[+] vector_append pass");
}

void test_vector_clear(void)
{
    vector vec;
    size_t value = 12345;
    vector_init(&vec, sizeof(size_t));
    vector_append(&vec, &value);

    vector_clear(&vec);
    assert(vec.size == 0);

    vector_destroy(&vec);
    puts("[+] vector_clear pass");
}

void test_vector_resize(void)
{
    vector vec;
    vector_init(&vec, sizeof(size_t));

    for (size_t i = 0; i < 10; i++)
    {
        vector_append(&vec, &i);
    }

    void* original_array = vec.array;

    assert(vector_resize(&vec, 1) == VECTOR_SUCCESS);
    assert(vec.max_size == VECTOR_DEFAULT_SIZE);
    assert(vec.array == original_array);

    assert(vector_resize(&vec, -1) == VECTOR_ALLOC_ERROR);
    assert(vec.array == original_array);

    assert(vector_resize(&vec, VECTOR_DEFAULT_SIZE + 1) == VECTOR_SUCCESS);
    assert(vec.max_size == VECTOR_DEFAULT_SIZE + 1);

    vector_destroy(&vec);
    puts("[+] vector_resize pass");
}

void test_vector_copy(void)
{
    vector vec1;
    vector vec2;

    memset(&vec2, 0, sizeof(vector));

    vector_init(&vec1, sizeof(size_t));

    for (size_t i = 0; i < 10; i++)
    {
        vector_append(&vec1, &i);
    }

    assert(vector_copy(&vec2, &vec1) == VECTOR_SUCCESS);

    assert(vec2.array != NULL);
    assert(vec1.array != vec2.array);
    assert(vec1.size == vec2.size);
    assert(memcmp(vec1.array, vec2.array, vec1.size * vec1.element_size) == 0);

    vector_destroy(&vec1);
    vector_destroy(&vec2);
    puts("[+] vector_copy pass");
}

void test_vector_pop(void)
{
    vector vec;
    struct test_struct test1;

    vector_init(&vec, sizeof(test1));
    assert(vector_pop(&vec, &test1) == VECTOR_INVALID_PARAM);

    strncpy(test1.str, "test string", sizeof(test1.str));
    test1.num = 12345;

    const size_t num_elements = 3;

    for (size_t i = 0; i < num_elements; i++)
    {
        vector_append(&vec, &test1);
    }

    struct test_struct test2;
    memset(&test2, 0, sizeof(test2));

    assert(vector_pop(&vec, &test2) == VECTOR_SUCCESS);
    assert(vec.size == num_elements - 1);
    assert(memcmp(&test1, &test2, sizeof(test1)) == 0);

    vector_destroy(&vec);
    puts("[+] vector_pop pass");
}

void test_vector_remove(void)
{
    vector vec;
    vector_init(&vec, sizeof(size_t));

    assert(vector_remove(&vec, 0) == VECTOR_INVALID_PARAM);

    size_t num_elements = 10;

    for (size_t i = 0; i < num_elements; i++)
    {
        vector_append(&vec, &i);
    }

    assert(vector_remove(&vec, num_elements) == VECTOR_INVALID_PARAM);
    assert(vector_remove(&vec, 0) == VECTOR_SUCCESS);
    assert(vec.size == num_elements - 1);

    size_t* array = (size_t*)vec.array;

    for (size_t i = 0; i < vec.size; i++)
    {
        assert(array[i] == i+1);
    }

    assert(vector_remove(&vec, vec.size-1) == VECTOR_SUCCESS);
    assert(vec.size == num_elements - 2);

    assert(vector_remove(&vec, 1) == VECTOR_SUCCESS);

    vector_destroy(&vec);
    puts("[+] vector_remove pass");
}

void test_vector_remove_swapback(void)
{
    vector vec;
    vector_init(&vec, sizeof(size_t));

    const size_t num_elements = 10;

    for (size_t i = 0; i < num_elements; i++)
    {
        vector_append(&vec, &i);
    }

    assert(vector_remove_swapback(&vec, num_elements) == VECTOR_INVALID_PARAM);

    assert(vector_remove_swapback(&vec, num_elements - 1) == VECTOR_SUCCESS);
    assert(vec.size == num_elements - 1);

    size_t* array = vec.array;

    for (size_t i = 0; i < num_elements - 1; i++)
    {
        // should not have swapped when removing the last element
        assert(array[i] == i);
    }

    vector_remove_swapback(&vec, 0);

    assert(vec.size == num_elements - 2);

    assert(array[0] == num_elements - 2);

    for (size_t i = 1; i < vec.size; i++)
    {
        assert(array[i] == i);
    }

    puts("[+] vector_remove_swapback pass");
}
