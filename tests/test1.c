#include <stdlib.h>
#include <stdio.h>

int main()
{
  /*printf("Running test 1 to test a simple malloc and free\n");

  char * ptr = ( char * ) malloc ( 65535 );
  free( ptr ); 

  return 0;*/

  // Modified version to test
  char * ptr1 = malloc(256);
  char * ptr2 = malloc(256);
  char * ptr3 = malloc(256);
  free(ptr3);
  free(ptr1);
  free(ptr2);
  ptr1 = malloc(512);
  free(ptr1);
}
