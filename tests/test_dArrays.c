#include <stdio.h>

#include "Daedalus.h"

static int test_destory( void );
static int test_resize( void );
static int test_grow( void );
static int test_append( void );
static int test_get( void );
static int test_pop( void );
static int test_insert( void );
static int test_remove( void );
static int test_clear( void );

int main(void)
{
  srand(time(NULL));

  printf("=== dArray Tests ===\n\n");
  
  if ( test_destory() ) return 1;
  if ( test_resize() ) return 1;
  if ( test_grow() ) return 1;
  if ( test_append() ) return 1;
  if ( test_get() ) return 1;
  if ( test_pop() ) return 1;
  if ( test_insert() ) return 1;
  //if ( test_remove() ) return 1;
  if ( test_clear() ) return 1;

  printf("=== All tests passed! ===\n");
  return 0;
}

static int test_destory( void )
{
  dArray_t* test_array = d_ArrayInit( 10, sizeof( int ) );

  int result = d_ArrayDestroy( test_array );
  test_array = NULL;
  
  if ( !result )
  {
    printf( "test_destory passed: %d\n", result );
    printf("\n");
    return 0;
  }

  printf( "test_destory failed: %d\n", result );
  printf("\n");
  return 1;
}

static int test_resize( void )
{
  dArray_t* test_array = d_ArrayInit( 10, sizeof( int ) );

  int old_capacity = test_array->capacity;
  d_ArrayResize(test_array);

  int new_capacity = test_array->capacity;
  
  if ( new_capacity == ( old_capacity * 2 ) )
  {
    printf( "test_resize passed: %d\n", test_array->capacity );
    d_ArrayDestroy( test_array );
    printf("\n");
    return 0;
  }

  printf( "test_resize failed: %d, %d\n", new_capacity, (old_capacity*2) );
  d_ArrayDestroy( test_array );
  printf("\n");
  return 1;
}

static int test_grow( void )
{
  dArray_t* test_array = d_ArrayInit( 10, sizeof( int ) );

  int old_capacity = test_array->capacity;
  int additional = 10;
  d_ArrayGrow( test_array, additional );

  int new_capacity = old_capacity + additional;
  
  if ( new_capacity == test_array->capacity )
  {
    printf( "test_grow passed: %d, %d\n", new_capacity, test_array->capacity );
    printf("\n");
    d_ArrayDestroy( test_array );
    return 0;
  }

  printf( "test_grow failed: %d, %d\n", new_capacity, test_array->capacity );
  d_ArrayDestroy( test_array );
  printf("\n");
  return 1;
}

static int test_append( void )
{
  int capacity = 10;
  dArray_t* test_array = d_ArrayInit( capacity, sizeof( int ) );
  int control_array[capacity];
  
  for ( int i = 0; i < capacity; i++ )
  {
    int* append_int = malloc( sizeof( int ) );
    *append_int = rand() % 100;
    d_ArrayAppend( test_array, append_int );
    control_array[i] = *append_int;
  }

  for ( int i = 0; i < capacity; i++ )
  {
    int* test_value = test_array->data[i];
    int control_value = control_array[i];

    if ( *test_value != control_value )
    {
      printf( "test_append %d failed: %d, %d\n", i, *test_value, control_value );
      d_ArrayDestroy( test_array );
      return 1;
    }
    
    printf( "test_append %d passed: %d, %d\n", i, *test_value, control_value );
  }

  d_ArrayDestroy( test_array );
  printf("\n");
  return 0;
}

static int test_get( void )
{
  int capacity = 10;
  dArray_t* test_array = d_ArrayInit( capacity, sizeof( int ) );
  int control_array[capacity];
  
  for ( int i = 0; i < capacity; i++ )
  {
    int* append_int = malloc( sizeof( int ) );
    *append_int = rand() % 100;
    d_ArrayAppend( test_array, append_int );
    control_array[i] = *append_int;
  }

  for ( int i = 0; i < capacity; i++ )
  {
    int* test_value = (int*)d_ArrayGet(test_array, i);
    int control_value = control_array[i];

    if ( *test_value != control_value )
    {
      printf( "test_get %d failed: %d, %d\n", i, *test_value, control_value );
      d_ArrayDestroy( test_array );
      return 1;
    }
    
    printf( "test_get %d passed: %d, %d\n", i, *test_value, control_value );
  }

  d_ArrayDestroy( test_array );
  printf("\n");
  return 0;
}

static int test_pop( void )
{
  int capacity = 10;
  dArray_t* test_array = d_ArrayInit( capacity, sizeof( int ) );
  int control_array[capacity];
  
  for ( int i = 0; i < capacity; i++ )
  {
    int* append_int = malloc( sizeof( int ) );
    *append_int = rand() % 100;
    d_ArrayAppend( test_array, append_int );
    control_array[i] = *append_int;
  }

  for ( int i = capacity-1; i >= 0; i-- )
  {
    int* test_value = (int*)d_ArrayPop(test_array);
    int control_value = control_array[i];

    if ( *test_value != control_value )
    {
      printf( "test_pop %d failed: %d, %d\n", i, *test_value, control_value );
      d_ArrayDestroy( test_array );
      return 1;
    }
    
    printf( "test_pop %d passed: %d, %d\n", i, *test_value, control_value );
  }

  d_ArrayDestroy( test_array );
  printf("\n");
  return 0;
}

static int test_insert( void )
{
  int capacity = 10;
  dArray_t* test_array = d_ArrayInit( capacity, sizeof( int ) );
  int control_array[capacity];
  
  for ( int i = 0; i < capacity; i++ )
  {
    int* append_int = malloc( sizeof( int ) );
    *append_int = rand() % 100;
    d_ArrayInsert( test_array, append_int, i );
    control_array[i] = *append_int;
  }

  for ( int i = 0; i < capacity; i++ )
  {
    int* test_value = (int*)d_ArrayGet(test_array, i);
    int control_value = control_array[i];

    if ( *test_value != control_value )
    {
      printf( "test_insert %d failed: %d, %d\n", i, *test_value, control_value );
      d_ArrayDestroy( test_array );
      return 1;
    }
    
    printf( "test_insert %d passed: %d, %d\n", i, *test_value, control_value );
  }

  d_ArrayDestroy( test_array );
  printf("\n");

  return 0;
}

static int test_remove( void )
{
  int capacity = 10;
  dArray_t* test_array = d_ArrayInit( capacity, sizeof( int ) );
  int control_array[capacity];
  
  for ( int i = 0; i < capacity; i++ )
  {
    int* append_int = malloc( sizeof( int ) );
    *append_int = rand() % 100;
    d_ArrayAppend( test_array, append_int );
    control_array[i] = *append_int;
  }

  for ( int i = 0; i < capacity; i++ )
  {
    int* test_value = (int*)d_ArrayGet(test_array, i);
    int control_value = control_array[i];

    if ( *test_value != control_value )
    {
      printf( "test_remove %d failed: %d, %d\n", i, *test_value, control_value );
      d_ArrayDestroy( test_array );
      return 1;
    }

    printf( "test_remove %d passed: %d, %d\n", i, *test_value, control_value );
    
    d_ArrayRemove(test_array, i);
    if ( test_array->data[i] != NULL )
    {
      printf( "test_remove %d failed\n", i );
      d_ArrayDestroy( test_array );
      return 1;
    }

    printf( "test_remove %d passed\n", i );
  }

  d_ArrayDestroy( test_array );
  printf("\n");
  return 0;
}

static int test_clear( void )
{
  int capacity = 10;
  dArray_t* test_array = d_ArrayInit( capacity, sizeof( int ) );
  
  for ( int i = 0; i < capacity; i++ )
  {
    int* append_int = malloc( sizeof( int ) );
    *append_int = rand() % 100;
    d_ArrayAppend( test_array, append_int );
  }
  
  d_ArrayClear( test_array );

  for ( int i = 0; i < capacity; i++ )
  {
    if ( test_array->data[i] != NULL )
    {
      printf( "test_clear %d failed\n", i );
      d_ArrayDestroy( test_array );
      return 1;
    }

    printf( "test_clear %d passed\n", i );
  }

  d_ArrayDestroy( test_array );
  printf("\n");
  return 0;
}

