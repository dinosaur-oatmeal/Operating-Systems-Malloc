#include <stdlib.h>
#include <stdio.h>

int main()
{
  /*printf("Running test 1 to test a simple malloc and free\n");

  char * ptr = ( char * ) malloc ( 65535 );
  free( ptr );*/

  // Modified version to test for report
  char * ptr = NULL;
  char * ptr1 = (char *)malloc(5000000000);
  char * ptr2 = (char *)malloc(1024);
  char * ptr3 = (char *)malloc(35);
  free(ptr1);

  char * ptr_arrayOne[14000];
  char * ptr_arrayTwo[10000];
  int j = 0;
  for (int i = 0; i < 14000; i++ )
  {
    ptr_arrayOne[i] = (char *) malloc(1024); 

    if(i % 10 == 0 && i > 0)
    {
      free(ptr_arrayOne[i]);
      ptr_arrayTwo[j] = (char *) malloc(2000);
      ptr = (char *) malloc(500);
      j++;
    }
  }

  char * ptr4 = malloc(10);
  free(ptr4);
  free(ptr3);

  char * ptr5 = malloc(1);
  free(ptr5);

  for(int k = 0; k < 10000; k++ )
  {
    if(k % 10 != 0)
    {
      free(ptr_arrayOne[k]);
    }
  }

  for(int m = 0; m < j; m++)
  {
    free(ptr_arrayTwo[m]);
  }

  free(ptr2);

  char ptr6 = (char *) malloc(700000);

  return 0;
}
