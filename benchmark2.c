#include <stdlib.h>
#include <stdio.h>

int main()
{
    //test multiple mallocs and frees
    char * ptr_array[1024];

    int i;
    for ( i = 0; i < 1024; i++ )
    {
        ptr_array[i] = ( char * ) malloc ( 1024 ); 
        
        ptr_array[i] = ptr_array[i];
    }

    for ( i = 0; i < 1024; i++ )
    {
        if( i % 2 == 0 )
        {
        free( ptr_array[i] );
        }
    }

    ptr = ( char * ) malloc ( 65535 );
    free( ptr ); 

    return 0;
}