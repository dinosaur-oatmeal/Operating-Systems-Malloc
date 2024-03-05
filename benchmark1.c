#include <stdlib.h>
#include <stdio.h>

int main()
{
  // test malloc free and realloc
  char * ptr1 = malloc(512);
  char * ptr2 = malloc(256);
  free(ptr1);
  char * ptr3 = malloc(256);
  char * ptr4 = realloc(ptr2, 512);
  free(ptr4);
  free(ptr3);
}