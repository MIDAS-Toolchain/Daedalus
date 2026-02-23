/**
 * @file dRawArrays.c
 *
 * Raw pointer arrays (void**) that grow as needed.
 * Stores raw pointers — caller manages element lifetime.
 *
 */

#include "Daedalus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// =============================================================================
// RAW ARRAY INITIALIZATION AND DESTRUCTION
// =============================================================================

dRawArray_t* d_RawArrayInit(int capacity) {

    dRawArray_t* array = (dRawArray_t*)malloc(sizeof(dRawArray_t));
    if (!array) return NULL;

    array->capacity = capacity;
    array->count = 0;

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

int d_RawArrayDestroy( dRawArray_t* array, uint8_t malloced ) {
  if ( !array ) return 1;

  if ( array->data )
  {
    for ( int i = 0; i < array->count; i++ )
    {
      if ( array->data[i] != NULL && malloced )
      {
        free( array->data[i] );
      }

      array->data[i] = NULL;
    }

    free( array->data );
  }

  array->data = NULL;
  array->capacity = 0;

  free( array );
  return 0;
}

// =============================================================================
// RAW ARRAY MEMORY MANAGEMENT
// =============================================================================

int d_RawArrayResize(dRawArray_t* array) {
    if (!array) return 1;

    array->capacity *= 2;
    void** new_data = realloc( array->data, array->capacity * sizeof( void* ) );
    if (!new_data) return 1;

    array->data = new_data;
    return 0;
}

int d_RawArrayGrow(dRawArray_t* array, int additional_slots)
{
  if (!array) return 1;
  if ( additional_slots <= 0 ) return 1;

  size_t new_capacity = array->capacity + additional_slots;
  void** new_data = realloc( array->data, new_capacity * sizeof( void* ) );
  if (!new_data) return 1;

  array->data = new_data;
  array->capacity = new_capacity;

  return 0;
}

// =============================================================================
// RAW ARRAY ELEMENT MANAGEMENT (to) and (from) ARRAY
// =============================================================================

int d_RawArrayAppend( dRawArray_t* array, void* data )
{
  if ( array == NULL || data == NULL )
  {
    return 1;
  }

  if ( array->count == array->capacity )
  {
    if ( d_RawArrayResize( array ) != 0 ) return 1;
  }

  array->data[array->count++] = data;

  return 0;
}

void* d_RawArrayGet(dRawArray_t* array, int index)
{
  if (!array || index >= array->count) {
    return NULL;
  }

  return array->data[index];
}

void* d_RawArrayPop(dRawArray_t* array)
{
  if (!array || array->count == 0) {
    return NULL;
  }

  void* rtn = array->data[array->count-1];
  array->data[array->count-1] = NULL;
  array->count--;

  return rtn;
}

// =============================================================================
// RAW ARRAY CAPACITY MANAGEMENT
// =============================================================================

int d_RawArrayEnsureCapacity(dRawArray_t* array, int min_capacity) {
    if (!array) {
        d_LogError("Attempted to ensure capacity of a NULL raw array.");
        return 1;
    }

    if (array->capacity >= min_capacity) {
        return 0;
    }

    int additional = min_capacity - array->capacity;

    if (d_RawArrayGrow(array, additional) != 0) {
        d_LogErrorF("Failed to resize raw array to ensure minimum capacity of %d.", min_capacity);
        return 1;
    }

    return 0;
}

// =============================================================================
// RAW ARRAY INSERTION AND REMOVAL
// =============================================================================

int d_RawArrayInsert(dRawArray_t* array, void* data, int index)
{
  if (!array || !data) {
    d_LogError("Invalid input: array or data is NULL for insert operation.");
    return 1;
  }

  if (index > array->count) {
    d_LogErrorF("Attempted to insert data at index %d, which is beyond current array count %d.",
                index, array->count);
    return 1;
  }

  if (array->count == array->capacity) {
    if (d_RawArrayResize(array) != 0) {
      d_LogErrorF("Failed to grow raw array for insert operation at index %d.", index);
      return 1;
    }
  }

  if (index < array->count)
  {
    for ( int i = array->count; i > index; i-- )
    {
      array->data[i] = array->data[i-1];
    }
  }

  array->data[index] = data;

  array->count++;
  return 0;
}

int d_RawArrayRemove(dRawArray_t* array, int index)
{
  if (!array) {
    d_LogError("Invalid input: array is NULL for remove operation.");
    return 1;
  }

  if (index >= array->count) {
    d_LogErrorF("Attempted to remove data at index %d, which is beyond current array count %d.",
                index, array->count);
    return 1;
  }

  array->data[index] = NULL;
  return 0;
}

int d_RawArrayRemoveByReference(dRawArray_t* array, void* index_ptr)
{
  if (!array) {
    d_LogError("Invalid input: array is NULL for remove operation.");
    return 1;
  }

  for ( int i = 0; i < array->count; i++ )
  {
    if ( array->data[i] == index_ptr )
    {
      array->data[i] = NULL;
      return 0;
    }
  }

  return 1;
}

int d_RawArrayClear(dRawArray_t* array, uint8_t free_elements)
{
  if (!array) {
    d_LogError("Attempted to clear a NULL raw array.");
    return 1;
  }

  if (free_elements)
  {
    for ( int i = 0; i < array->count; i++ )
    {
      free( array->data[i] );
      array->data[i] = NULL;
    }
  }

  array->count = 0;

  return 0;
}
