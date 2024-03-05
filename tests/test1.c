#include <stdlib.h>
#include <stdio.h>

int main()
{
  /*printf("Running test 1 to test a simple malloc and free\n");

  char * ptr = ( char * ) malloc ( 65535 );
  free( ptr );*/

  // Modified version to test for report
  printf("testing malloc, free and realloc");
  char * ptr1 = malloc(512);
  char * ptr2 = malloc(256);
  free(ptr1);
  char * ptr3 = malloc(256);
  char * ptr4 = realloc(ptr2, 512);
  free(ptr4);
  free(ptr3);

  printf("testing printing in an array");
  char * ptr_array[15000];
  int i;
  for ( i = 0; i < 15000; i++ )
  {
    ptr_array[i] = ( char * ) malloc ( 1024 ); 
    
    ptr_array[i] = ptr_array[i];
  }

  for ( i = 0; i < 15000; i++ )
  {
    if( i % 2 == 0 )
    {
      free( ptr_array[i] );
    }
  }

  return 0;
}
