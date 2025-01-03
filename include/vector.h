#include <stddef.h>

/* Do not modify members of this struct.
 * Append or delete operations must go through the vector API.
 * You should cast array to a pointer of your type to access, update, or sort existing elements.
 * You should use size to loop through elements.
 */
typedef struct vector
{
    void* array;
    size_t size;
    size_t max_size;
    size_t element_size;
} vector;

// Vector API return values
enum VECTOR_STATUS
{
    VECTOR_SUCCESS,
    VECTOR_INVALID_PARAM,
    VECTOR_ALLOC_ERROR
};

// Initializes a vector struct
// Allocates an array of element_size * a reasonable initial number of elements.
int vector_init(vector* vec, size_t elememt_size);

// Adds an element to the end of the array, reallocating the array if it is full.
// Warning: This may invalidate an array pointer you have saved.
// After appending something to the array, you must recast array to a pointer of your type before accessing the data.
int vector_append(vector* vec, void* data);

// Frees the vector's array.
// Does not free the vector object; you must free the vector object if it was malloc'd.
int vector_destroy(vector* vec);

// Sets the vector size to 0.
// Does not release any resources.
int vector_clear(vector* vec);

// Resizes the vector's array to accommodate new_size elements.
// Warning: This will invalidate an array pointer you have saved.
// After resizing, you must recast array to a pointer of your type before accessing the data.
int vector_resize(vector* vec, size_t new_size);

// Initializes dest with a new array equal in size to src, and copys the data from src.array to dest.array.
// Does not free resources in dest. Do not pass an initialized vector in dest without calling vector_destroy(dest) first.
int vector_copy(vector* dest, vector* src);

// Copys the last element to out_data and removes it from the vector.
int vector_pop(vector* vec, void* out_data);

// Removes the element at index and shifts the remaining elements forward
int vector_remove(vector* vec, size_t index);
