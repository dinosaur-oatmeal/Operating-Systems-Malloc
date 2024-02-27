//Name: Will Maberry

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// used for INT_MAX and INT_MIN
#include <limits.h>

#define ALIGN4(s)         (((((s) - 1) >> 2) << 2) + 4)
#define BLOCK_DATA(b)     ((b) + 1)
#define BLOCK_HEADER(ptr) ((struct _block *)(ptr) - 1)

static int atexit_registered = 0;
static int num_mallocs       = 0;
static int num_frees         = 0;
static int num_reuses        = 0;
static int num_grows         = 0;
static int num_splits        = 0;
static int num_coalesces     = 0;
static int num_blocks        = 0;
static int num_requested     = 0;
static int max_heap          = 0;

// reuse is how many times growheap isn't called
// blocks are how many blocks we have at the end of the program?
//    loop through list at the end of the program and add the number of nodes
// requested is the toal amount of memory requested throughout the program's execution?
//    add input size everytime malloc is called

// Helpful commands (best fit)
// env LD_PRELOAD=lib/libmalloc-bf.so tests/bfwf
// set exec-wrapper env LD_PRELOAD=./lib/libmalloc-bf.so

/*
 *  \brief printStatistics
 *
 *  \param none
 *
 *  Prints the heap statistics upon process exit.  Registered
 *  via atexit()
 *
 *  \return none
 */
void printStatistics( void )
{
  printf("\nheap management statistics\n");
  printf("mallocs:\t%d\n", num_mallocs );
  printf("frees:\t\t%d\n", num_frees );
  printf("reuses:\t\t%d\n", num_reuses );
  printf("grows:\t\t%d\n", num_grows );
  printf("splits:\t\t%d\n", num_splits );
  printf("coalesces:\t%d\n", num_coalesces );
  printf("blocks:\t\t%d\n", num_blocks );
  printf("requested:\t%d\n", num_requested );
  printf("max heap:\t%d\n", max_heap );
}

// TO DO: nextFit
// can't coalesce final two blocks because second one points to NULL
// can't malloc size < ptr->size (needs to split somehow)
// splits + grows = mallocs

struct _block 
{
   size_t  size;         /* Size of the allocated _block of memory in bytes */
   struct _block *next;  /* Pointer to the next _block of allocated memory  */
   bool   free;          /* Is this _block free?                            */
   char   padding[3];    /* Padding: IENTRTMzMjAgU3jMDEED                   */
};

/* Free list to track the _blocks available */
struct _block *heapList = NULL;

/*
 * \brief findFreeBlock
 *
 * \param last pointer to the linked list of free _blocks
 * \param size size of the _block needed in bytes 
 *
 * \return a _block that fits the request or NULL if no free _block matches
 *
 * \TODO Implement Next Fit
 * \TODO Implement Best Fit
 * \TODO Implement Worst Fit
 */
 
struct _block *findFreeBlock(struct _block **last, size_t size) 
{
   struct _block *curr = heapList;

#if defined FIT && FIT == 0
   /* First fit */

   // While we haven't run off the end of the linked list and
   // while the current node we point to isn't free or isn't big enough
   // then continue to iterate over the list.  This loop ends either
   // with curr pointing to NULL, meaning we've run to the end of the list
   // without finding a node or it ends pointing to a free node that has enough
   // space for the request.

   // loop through list to find first free block
   while(curr && !(curr->free && curr->size >= size)) 
   {
      *last = curr;
      curr  = curr->next;
   }
#endif

#if defined BEST && BEST == 0

   *last = curr;
   struct _block *win = NULL;
   int winDiff = INT_MAX;

   // loop through list and find remaining size of blocks
   while(curr)
   {
      if(curr->free && curr->size >= size)
      {
         int remainder = (int)curr->size - (int)size;

         // find smallest remainder
         if(remainder < winDiff)
         {
            winDiff = remainder;
            win = curr;
         }
      }

      *last = curr;
      curr = curr->next;
   }

   // set curr to block with smallest remainder
   curr = win;
#endif

#if defined WORST && WORST == 0

   *last = curr;
   struct _block *win = NULL;
   int winDiff = INT_MIN;

   // loop through list and find remaining size of blocks
   while(curr)
   {
      if(curr->free && curr->size >= size)
      {
         int remainder = (int)curr->size - (int)size;

         // find largest remainder
         if(remainder > winDiff)
         {
            winDiff = remainder;
            win = curr;
         }
      }

      *last = curr;
      curr = curr->next;
   }

   // set curr to block with largest remainder
   curr = win;
#endif

// \TODO Put your Next Fit code in this #ifdef block
#if defined NEXT && NEXT == 0
   /** \TODO Implement next fit here */
#endif

   return curr;
}

/*
 * \brief growheap
 *
 * Given a requested size of memory, use sbrk() to dynamically 
 * increase the data segment of the calling process.  Updates
 * the free list with the newly allocated memory.
 *
 * \param last tail of the free _block list
 * \param size size in bytes to request from the OS
 *
 * \return returns the newly allocated _block of NULL if failed
 */
struct _block *growHeap(struct _block *last, size_t size) 
{
   // add to max_heap
   max_heap += size;

   // Request more space from OS
   struct _block *curr = (struct _block *)sbrk(0);
   struct _block *prev = (struct _block *)sbrk(sizeof(struct _block) + size);

   assert(curr == prev);

   // OS allocation failed
   if (curr == (struct _block *)-1) 
   {
      return NULL;
   }

   // Update heapList if not set
   if (heapList == NULL) 
   {
      heapList = curr;
   }

   // Attach new _block to previous _block
   if (last) 
   {
      last->next = curr;
   }

   /* Update _block metadata:
      Set the size of the new block and initialize the new block to "free".
      Set its next pointer to NULL since it's now the tail of the linked list.
   */

   curr->size = size;
   curr->next = NULL;
   curr->free = false;
   return curr;
}

/*
 * \brief malloc
 *
 * finds a free _block of heap memory for the calling process.
 * if there is no free _block that satisfies the request then grows the 
 * heap and returns a new _block
 *
 * \param size size of the requested memory in bytes
 *
 * \return returns the requested memory allocation to the calling process 
 * or NULL if failed
 */
void *malloc(size_t size) 
{
   // call statistic variables
   if( atexit_registered == 0 )
   {
      atexit_registered = 1;
      atexit( printStatistics );
   }

   // Align to multiple of 4
   size = ALIGN4(size);

   // Handle 0 size
   if (size == 0) 
   {
      return NULL;
   }

   // add to the number of mallocs the program calls
   num_mallocs++;

   // Look for free _block.  If a free block isn't found then we need to grow our heap.
   struct _block *last = heapList;
   struct _block *next = findFreeBlock(&last, size);

   /* TODO: If the block found by findFreeBlock is larger than we need then:
            If the leftover space in the new block is greater than the sizeof(_block)+4 then
            split the block.
            If the leftover space in the new block is less than the sizeof(_block)+4 then
            don't split the block.
   */

   // split functionality
   if(next != NULL && (next->size) > size)
   {
      if((next->size - size) > (sizeof(struct _block) + 4))
      {
         // create a temp pointer and update input (next) pointer
         struct _block *tempPointer = next->next;
         //printf("%ld %ld\n", sizeof(next), sizeof(int));
         next->next = (struct _block * )((long long)next + (long long)size + ((long long)sizeof(struct _block)));

         // update pointers, sizes, and values of old and new blocks
         next->next->next = tempPointer;
         next->next->size = next->size - size;
         next->next->free = true;
         next->size = size;
         num_splits++;
      }
   }

   // Could not find free _block, so grow heap
   if (next == NULL) 
   {
      next = growHeap(last, size);
      num_grows++;
   }

   // Could not find free _block or grow heap, so just return NULL
   if (next == NULL) 
   {
      return NULL;
   }
   
   // Mark _block as in use
   next->free = false;

   // Return data address associated with _block to the user
   return BLOCK_DATA(next);
}

/*
 * \brief free
 *
 * frees the memory _block pointed to by pointer. if the _block is adjacent
 * to another _block then coalesces (combines) them
 *
 * \param ptr the heap memory to free
 *
 * \return none
 */
void free(void *ptr) 
{
   if (ptr == NULL) 
   {
      return;
   }

   // Make _block as free
   struct _block *curr = BLOCK_HEADER(ptr);
   assert(curr->free == 0);
   curr->free = true;
   num_frees++;

   /* TODO: Coalesce free _blocks.  If the next block or previous block 
            are free then combine them with this block being freed.
   */

   curr = heapList;

   // loop through list
   while(curr)
   {
      // find adjacent blocks that are free, update pointers, and combine the blocks
      if(curr->free && curr->next && curr->next->free)
      {
         //printf("curr size %d\ncurr next size %d", (int)curr->size, (int)curr->next->size);
         curr->size += curr->next->size + sizeof(struct _block);
         curr->next = curr->next->next;
         num_coalesces++;
      }

      curr = curr->next;
   }
}

void *calloc( size_t nmemb, size_t size )
{
   // find total size of inputs
   size_t totalSize = nmemb * size;

   // malloc total size of inputs and set values to 0
   struct _block *tempPointer = malloc(totalSize);
   memset(tempPointer, 0, totalSize);
   
   return BLOCK_DATA(heapList);
}

void *realloc( void *ptr, size_t size )
{
   num_reuses++;

   // size 0 is free
   if(size == 0)
   {
      free(ptr);
   }

   // invalid pointer is NULL
   else if(ptr == NULL)
   {
      return NULL;
   }

   // size > old size
   else if(size > sizeof(ptr))
   {
      struct _block *newPointer = malloc(size);

      // copy content from oldPointer and free it
      memcpy(newPointer, ptr, size);
      free(ptr);

      return newPointer;
   }
   
   // requested size < allocated
   return ptr;
}



/* vim: IENTRTMzMjAgU3ByaW5nIDIwMjM= -----------------------------------------*/
/* vim: set expandtab sts=3 sw=3 ts=6 ft=cpp: --------------------------------*/