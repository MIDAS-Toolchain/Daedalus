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
  printf("=== dArray Tests ===\n\n");
  
  int result = 0;
  if ( test_destory() ) return 1;

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
    return 0;
  }

  printf( "test_destory failed: %d\n", result );
  return 1;
}

static int test_resize( void )
{

  return 0;
}

static int test_grow( void )
{

  return 0;
}

static int test_append( void )
{

  return 0;
}

static int test_get( void )
{

  return 0;
}

static int test_pop( void )
{

  return 0;
}

static int test_insert( void )
{

  return 0;
}

static int test_remove( void )
{

  return 0;
}

static int test_clear( void )
{

  return 0;
}

