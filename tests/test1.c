#include <stdlib.h>
#include <stdio.h>

int main()
{
  /*printf("Running test 1 to test a simple malloc and free\n");

  char * ptr = ( char * ) malloc ( 65535 );
  free( ptr ); 

  return 0;*/

  // Modified version to test
  /*char * ptr1 = malloc(256);
  char * ptr2 = malloc(256);
  char * ptr3 = malloc(256);
  free(ptr3);
  free(ptr1);
  free(ptr2);
  ptr1 = malloc(4);
  free(ptr1);*/

  char * ptr1 = malloc(sizeof(int));
  char * ptr2 = malloc(sizeof(int));
  char * ptr3 = malloc(sizeof(int));
  char * ptr4 = malloc(sizeof(int));
  char * ptr5 = malloc(sizeof(int));

  free(ptr1);
  free(ptr2);
  free(ptr3);
  free(ptr4);
  free(ptr5);
}
