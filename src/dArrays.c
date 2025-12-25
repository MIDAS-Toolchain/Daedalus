/**
 * @file dArrays.c
 *
 * Dynamic Arrays that grow as needed.
 * 
 */

#include "Daedalus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// =============================================================================
// DYNAMIC ARRAY INITIALIZATION AND DESTRUCTION
// =============================================================================

dArray_t* d_ArrayInit(int capacity, size_t element_size) {
    if (element_size == 0) return NULL;

    dArray_t* array = (dArray_t*)malloc(sizeof(dArray_t));
    if (!array) return NULL;

    array->capacity = capacity;
    array->count = 0;
    array->element_size = element_size;

    // Only allocate memory if the initial capacity is greater than zero.
    if (capacity > 0) {
        array->data = malloc( array->capacity * sizeof( void* ) );
        if (!array->data) {
            free(array);
            return NULL;
        }
    } else {
        array->data = NULL;
    }
    return array;
}

int d_ArrayDestroy(dArray_t* array) {
  if ( !array ) return 1;

  for ( int i = 0; i < array->count; i++ )
  {
    free( array->data[i] );
  }
  
  free( array->data );

  array->data = NULL;
  array->element_size = array->capacity = 0;

  free( array );
  
  return 0;
}

// =============================================================================
// DYNAMIC ARRAY MEMORY MANAGEMENT
// =============================================================================

int d_ArrayResize(dArray_t* array) {
    if (!array) return 1;

    // If new size is 0, free the data and reset.
    int new_size = array->capacity * 2;
    array->data = realloc( array->data, new_size * sizeof( void* ) );

    return 0; // Success
}

int d_ArrayGrow(dArray_t* array, int additional_bytes) {
    if (!array) return 1;
    if ( additional_bytes <= 0 ) return 1;

    size_t current_bytes = array->capacity * sizeof( void* );
    array->data = realloc( array->data,
                           ( current_bytes +
                           ( additional_bytes * sizeof( void* ) ) ) );
    return 0;
}

// =============================================================================
// DYNAMIC ARRAY ELEMENT MANAGEMENT (to) and (from) ARRAY
// =============================================================================

int d_ArrayAppend( dArray_t* array, void* data )
{
  if ( array == NULL || data == NULL )
  {
    return 1;
  }

  if ( array->count == array->capacity )
  {
    if ( d_ArrayResize( array ) != 0 ) return 1;
  }
  array->data[array->count++] = data;

  return 0;
}

void* d_ArrayGet(dArray_t* array, int index)
{
    if (!array || index >= array->count) {
        return NULL;
    }

    return array->data[index];
}

void* d_ArrayPop(dArray_t* array) {
    if (!array || array->count == 0) {
        return NULL;
    }
    return array->data[array->count--];
}

// =============================================================================
// DYNAMIC ARRAY CAPACITY MANAGEMENT
// =============================================================================

int d_ArrayEnsureCapacity(dArray_t* array, int min_capacity) {
    if (!array) {
        d_LogError("Attempted to ensure capacity of a NULL dynamic array.");
        return 1;
    }

    if (array->element_size == 0) {
        d_LogError("Cannot ensure capacity for array with zero element size.");
        return 1;
    }

    if (array->capacity >= min_capacity) {
        d_LogDebugF("Array already has sufficient capacity (%zu) for requested minimum (%zu).", 
                    array->capacity, min_capacity);
        return 0;
    }

    int old_capacity = array->capacity;
    int new_capacity = min_capacity;

    if (d_ArrayGrow(array, new_capacity) != 0) {
        d_LogErrorF("Failed to resize array to ensure minimum capacity of %zu.", min_capacity);
        return 1;
    }

    d_LogInfoF("Array capacity increased from %zu to %zu to meet minimum %zu.", 
               old_capacity, array->capacity, min_capacity);
    return 0;
}

// =============================================================================
// DYNAMIC ARRAY INSERTION AND REMOVAL
// =============================================================================

int d_ArrayInsert(dArray_t* array, void* data, int index)
{
  if (!array || !data) {
    d_LogError("Invalid input: array or data is NULL for insert operation.");
    return 1;
  }

  if (index > array->count) {
    d_LogErrorF("Attempted to insert data at index %zu, which is beyond current array count %zu.", 
                index, array->count);
    return 1;
  }

  if (array->count == array->capacity) {
    if (d_ArrayResize(array) != 0) {
      d_LogErrorF("Failed to grow array for insert operation at index %zu.", index);
      return 1;
    }
  }

  if (index < array->count)
  {
    void* temp_holder = NULL;
    for ( int i = index; i < array->count; i++ )
    {
      temp_holder = array->data[i];
      array->data[i+1] = temp_holder;
    }
  }
  
  array->data[index] = data;

  array->count++;
  return 0;
}

int d_ArrayRemove(dArray_t* array, int index)
{
  if (!array) {
    d_LogError("Invalid input: array is NULL for remove operation.");
    return 1;
  }

  if (index >= array->count) {
    d_LogErrorF("Attempted to remove data at index %zu, which is beyond current array count %zu.", 
                index, array->count);
    return 1;
  }

  if (index < array->count - 1)
  {
    void* temp_holder = NULL;
    
    free( array->data[index] );

    for ( int i = index; i < array->count; i++ )
    {
      temp_holder = array->data[i+1];
      array->data[i] = temp_holder;
    }
  }

  array->count--;
  return 0;
}

int d_ArrayClear(dArray_t* array)
{
  if (!array) {
    d_LogError("Attempted to clear a NULL dynamic array.");
    return 1;
  }

  for ( int i = 0; i < array->count; i++ )
  {
    free( array->data[i] );
  }

  free( array->data );

  array->data = NULL;
  array->element_size = array->capacity = array->count = 0;

  return 0;
}

