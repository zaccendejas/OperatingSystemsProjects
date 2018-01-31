/* malloc.c: simple memory allocator -----------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* Macros --------------------------------------------------------------------*/

#define ALIGN4(s)           (((((s) - 1) >> 2) << 2) + 4)
#define BLOCK_DATA(b)       ((b) + 1)
#define BLOCK_HEADER(ptr)   ((struct block *)(ptr) - 1)

/* Block structure -----------------------------------------------------------*/

struct block {
    size_t        size;
    struct block *next;
    bool          free;
};

/* Global variables ----------------------------------------------------------*/

struct block *FreeList = NULL;

/* TODO: Add counters for mallocs, frees, reuses, grows */
int MALLOC = 0;
int FREE = 0;
int REUSE = 0;
int REQUEST = 0;
int SPLIT = 0;
int COALESCE = 0;
int NUMBLOCKSINFREE = 0;
int TOTALMEM = 0;
int MAXHEAPSIZE = 0;
struct block *lastNextFit = NULL; // Used to keep track of the last block accessed and to control looping through the list during Next Fit
int CoutCopy;
int CALLONCE = 1;

//~~~~~~~~~~~//
// FUNCTIONS //
//~~~~~~~~~~~//
void DisplayCounters(){
	dup2(CoutCopy, 1);	// Restore stdout
	close(CoutCopy);
	
	char Output[BUFSIZ];
	sprintf(Output, "%-11s%d\n%-11s%d\n%-11s%d\n%-11s%d\n%-11s%d\n%-11s%d\n%-11s%d\n%-11s%d\n%-11s%d", "mallocs:", MALLOC, "frees:", FREE, "reuses:", REUSE, "grows:", REQUEST, "splits:", SPLIT, "coalesces:", COALESCE, "blocks:", NUMBLOCKSINFREE, "requested:", TOTALMEM, "max heap:", MAXHEAPSIZE);
	puts(Output);

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// REDUCES THE SIZE OF THE FREE BLOCK TO THE REQUESTED SIZE AND INSERTS A NEW BLOCK AHEAD OF IT WITH THE REMAINING SIZE //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void Split(struct block **BigBlock, size_t Size){
	size_t FreeSize = (*BigBlock)->size - Size - sizeof(struct block);
	(*BigBlock)->size = Size;
    struct block *SplittingBlock = (void*)BLOCK_DATA(*BigBlock) + Size; // Have the pointer pointing to where the split should end (using the same logic in how sbrk creates two bloks when called twice in a row).
    SplittingBlock->next = (*BigBlock)->next;
	(*BigBlock)->next = SplittingBlock;
	SplittingBlock->size = FreeSize;
	SplittingBlock->free = true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// COMBINES THE SIZES OF TWO BLOCKS AND MAKES THE SECOND POINTER IRRELEVANT //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void Coalesce(struct block **SmallBlock){
	size_t FreeSize = (*SmallBlock)->next->size + sizeof(struct block);
	(*SmallBlock)->size += FreeSize;
	(*SmallBlock)->next = (*SmallBlock)->next->next;
}

/* Find free block -----------------------------------------------------------*/

struct block *find_free(struct block **last, size_t size) {
    struct block *curr = FreeList;

#if defined FIT && FIT == 0
    /* First fit */
    while (curr && !(curr->free && curr->size >= size)) {
        *last = curr;
        curr  = curr->next;
    }
#endif
    /* TODO: Support other policies */
    // Next Fit
#if defined FIT && FIT == 1 
    if (lastNextFit == NULL) lastNextFit = FreeList; // needed for first time
    curr = lastNextFit;
    struct block *tmp = lastNextFit;
    while (curr && !(curr->free && curr->size >= size)){
        if (!lastNextFit->next){
            *last = lastNextFit; // tail
            lastNextFit = FreeList;
        } 
        else {
            lastNextFit = lastNextFit->next;
        }
        curr = lastNextFit;
        if (curr == tmp) curr = NULL; // we need to grow the heap
    }
#endif
    // Best Fit
#if defined FIT && FIT == 2
    struct block *smallest = NULL;
    while (curr){
        if(curr->free && curr->size >= size){
            if (smallest){
                if(smallest->size > curr->size) smallest = curr;
            }
            else {
                smallest = curr;
            }
        }
        if (!curr->next) *last = curr; 
        curr = curr->next;
    }
    if (smallest) curr = smallest;
    else curr = NULL; // grow heap
#endif
    // Worst Fit
#if defined FIT && FIT == 3
    struct block *largest = NULL;
    while (curr){
        if(curr->free && curr->size >= size){
            if (largest){
                if(largest->size < curr->size) largest = curr;
            }
            else {
                largest = curr;
            }
        }
        if (!curr->next) *last = curr; 
        curr = curr->next;
    }
    if (largest) curr = largest;
    else curr = NULL; // grow heap

#endif

    return curr;
}

/* Grow heap -----------------------------------------------------------------*/

struct block *grow_heap(struct block *last, size_t size) {
    /* Request more space from OS */
    struct block *curr = (struct block *)sbrk(0);
    struct block *prev = (struct block *)sbrk(sizeof(struct block) + size);

    assert(curr == prev);

    /* OS allocation failed */
    if (curr == (struct block *)-1) {
        return NULL;
    }

	MAXHEAPSIZE += (size);

    /* Update FreeList if not set */
    if (FreeList == NULL) {
        FreeList = curr;
    }

    /* Attach new block to prev block */
    if (last) {
        last->next = curr;
    }

    /* Update block metadata */
    curr->size = size;
    curr->next = NULL;
    curr->free = false;
    return curr;
}

/* Allocate space ------------------------------------------------------------*/

void *malloc(size_t size) {

	CoutCopy = dup(1);
    if (CALLONCE == 1){
		atexit(DisplayCounters);
        CALLONCE = 0;
    }

    /* Align to multiple of 4 */
    size = ALIGN4(size);
	TOTALMEM += size;

    /* Handle 0 size */
    if (size == 0) {
        return NULL;
    }

    /* Look for free block */
    struct block *last = FreeList;
    struct block *next = find_free(&last, size);


    /* Could not find free block, so grow heap */
    if (next == NULL) {
        next = grow_heap(last, size);
        if (next != NULL) NUMBLOCKSINFREE++;
		REQUEST++;
    }else{
		REUSE++;
    	/* TODO: Split free block? */
         
		if (next->size > (size + sizeof(struct block))){
			Split(&next, size);
			SPLIT++;
			NUMBLOCKSINFREE++;
		}
        
    }

    /* Could not find free block or grow heap, so just return NULL */
    if (next == NULL) {
        return NULL;
    }
    
    /* Mark block as in use */
    next->free = false;

    /* Return data address associated with block */
    MALLOC++;
    return BLOCK_DATA(next);
}

/* Reclaim space -------------------------------------------------------------*/

void free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    /* Make block as free */
    struct block *curr = BLOCK_HEADER(ptr);
    assert(curr->free == 0);
    curr->free = true;
	FREE++;

    /* TODO: Coalesce free blocks? */
    
     if (curr->next){
		 if (curr->next->free){
			 Coalesce(&curr);
			 COALESCE++;
			 NUMBLOCKSINFREE--;
			 }
	}
    
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=cpp: --------------------------------*/
