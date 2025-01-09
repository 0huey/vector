#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#define TEST_CALLOC(addr, func, count, size) {\
    if(!addr) {\
        perror(func);\
        fprintf(stderr, "calloc error with params: count=%lu, size=%lu\n", count, size);\
        return VECTOR_ALLOC_ERROR;}}

size_t VECTOR_DEFAULT_SIZE = 100;
double VECTOR_RESIZE_FACTOR = 2.0;

int vector_init(vector* vec, size_t element_size)
{
    return vector_init_with_size(vec, element_size, VECTOR_DEFAULT_SIZE);
}

int vector_init_with_size(vector* vec, size_t element_size, size_t num_elements)
{
    if (!vec || !element_size || !num_elements)
    {
        return VECTOR_INVALID_PARAM;
    }

    vec->array = calloc(num_elements, element_size);
    TEST_CALLOC(vec->array, "vector_init_with_size", num_elements, element_size);

    vec->size = 0;
    vec->max_size = num_elements;
    vec->element_size = element_size;

    return VECTOR_SUCCESS;
}

int vector_append(vector* vec, void* data)
{
    if (!vec || !data)
    {
        return VECTOR_INVALID_PARAM;
    }

    assert(vec->size <= vec->max_size);

    if (vec->size == vec->max_size)
    {
        int status = vector_resize(vec, vec->max_size * VECTOR_RESIZE_FACTOR);

        if (status != VECTOR_SUCCESS)
        {
            return status;
        }
    }

    intptr_t dest = (intptr_t)vec->array + vec->element_size * vec->size++;

    memcpy((void*)dest, data, vec->element_size);

    return VECTOR_SUCCESS;
}

int vector_destroy(vector* vec)
{
    if (!vec)
    {
        return VECTOR_INVALID_PARAM;
    }

    free(vec->array);
    memset(vec, 0, sizeof(vector));
    return VECTOR_SUCCESS;
}

int vector_clear(vector* vec)
{
    if (!vec)
    {
        return VECTOR_INVALID_PARAM;
    }

    vec->size = 0;
    return VECTOR_SUCCESS;
}

int vector_resize(vector* vec, size_t new_size)
{
    if (!vec || !new_size)
    {
        return VECTOR_INVALID_PARAM;
    }

    if (new_size <= vec->size)
    {
        return VECTOR_SUCCESS;
    }

    void* new_array = realloc(vec->array, vec->element_size * new_size);
    TEST_CALLOC(new_array, "vector_resize", new_size, vec->element_size);

    vec->array = new_array;
    vec->max_size = new_size;

    return VECTOR_SUCCESS;
}

int vector_copy(vector* dest, vector* src)
{
    if (!dest || !src)
    {
        return VECTOR_INVALID_PARAM;
    }

    void* new_array = calloc(src->max_size, src->element_size);
    TEST_CALLOC(new_array, "vector_copy", src->max_size, src->element_size);

    memcpy(dest, src, sizeof(vector));
    dest->array = new_array;
    memcpy(dest->array, src->array, dest->size * dest->element_size);

    return VECTOR_SUCCESS;
}

int vector_pop(vector* vec, void* out_data)
{
    if (!vec || !out_data || !vec->size)
    {
        return VECTOR_INVALID_PARAM;
    }

    intptr_t src = (intptr_t)vec->array + --vec->size * vec->element_size;
    memcpy(out_data, (void*)src, vec->element_size);

    return VECTOR_SUCCESS;
}

int vector_remove(vector* vec, size_t index)
{
    if (!vec || index >= vec->size)
    {
        return VECTOR_INVALID_PARAM;
    }

    intptr_t dest = (intptr_t)vec->array + index * vec->element_size;
    intptr_t src = dest + vec->element_size;
    size_t len = (size_t)( ((intptr_t)vec->array + vec->size * vec->element_size) - src );

    // testing that len didn't underflow
    // the math should work out that len==0 when removing the last element
    assert(len < vec->size * vec->element_size);

    memmove((void*)dest, (void*)src, len);
    vec->size--;

    return VECTOR_SUCCESS;
}
