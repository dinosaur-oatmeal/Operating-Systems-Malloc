#include <stdlib.h>
#include <stdio.h>

int main()
{
  /*printf("Running test 1 to test a simple malloc and free\n");

  char * ptr = ( char * ) malloc ( 65535 );
  free( ptr ); 

  return 0;*/

  // Modified version to test
  char * ptr1 = malloc(512);
  char * ptr2 = malloc(256);
  free(ptr1);
  char * ptr3 = malloc(256);
  char * ptr4 = realloc(ptr2, 512);
  free(ptr4);
  free(ptr3);

  /*char * ptr1 = (char *)malloc(5000);
  char * array[5000];
  for(int i = 0; i < 5000; i++)
  {
    array[i] = (char *)realloc(ptr1, i);
    array[i] = array[i];
  }*/
}
