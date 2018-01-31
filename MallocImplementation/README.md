CSE.30341.FA17: Project 04
==========================

This is the documentation for [Project 04] of [CSE.30341.FA17].

Members
-------

1. Zac Cendejas (zcendeja@nd.edu)
2. Will Fritz (wfritz@nd.edu)

Design
------

> 1. You will need to implement splitting of free blocks:
>
>   - When should you split a block?
>
>   - How should you split a block?

Response.

~) We need to split a block before anytime we return a data location to a user. This should be done after a free block is found and is the one the current 
policy is deciding to use.
~) We should split a block by adding a new block after the current free block. The current free block has its next updated to this new block,
the new block points to the original next of the current free block, the size of the current free block is changed to the size malloc needs to allocate
for this call (header and data total), and the size of the new block is the remaining free space.

> 2. You will need to implement coalescing of free blocks:
>
>   - When should you coalescing block?
>
>   - How should you coalesce a block?

Response.

~) We should coalesce a block whenever there are at least two free blocks next to each other in the free list. 
~) We should coalesce blocks by checking if the next block in the list is free after a block is free'd. If it is, then we can update the header of the
block we just free'd so that it points to the address it's next block was pointing to as its next block. We can then add the size of the block next to
the free'd block to the free'd block. The free'd block should now be representative of the combination of itself and its next block. The old next block
is now forgotten about and unused.

> 3. You will need to implement Next Fit.
>
>   - What information do you need to perform a Next Fit?
>
>   - How would you implement Next Fit?

Response.

~) In order to perform Next Fit we need to keep track of the last block that was accessed. We also need to keep track of which blocks are free and
what blocks are in use.
~) We will implement Next Fit by using a similar loop to First Fit that looks for a free block to return. In Next Fit the loop will start looking
at the pointer of the last block that was accessed and it will loop through the list until it either reaches the end or finds a free block with
the right size requirements. We will also have to update a pointer in this loop in the case that we do reach the end of our list. (This pointer
can then be used in grow_heap() to keep the list connected) If this loop ends without finding a free block then we will loop through the list
again starting at the head, searching until we reach the starting point of our first loop. If this loop is unsuccessful in finding a free block
then we will need to grow the heap. When a free block is found we return a pointer to it so that the rest of malloc can handle splitting the block.

> 4. You will need to implement Best Fit.
>
>   - What information do you need to perform a Best Fit?
>
>   - How would you implement Best Fit?

Response.

~) We will need to keep track of which block has the currently has the size closest to the desired number. We will also need to keep track of which
block we are looking at in case we never find a fit and need to pass the tail of the list t grow_heap(). 
~) We will implement Best Fit by looping through the list starting at the head. On the first iteration where a free block with an acceptable size is
found, the loop will assign the size of this block to a variable and set a pointer equal to this block. Each iteration that follows where another
possible block is found will compare that blocks size to the size that is stored in the local variable. If the current block has a smaller size then
the variable is updated to that size and the block pointer is set equal to the current block. If the size is an exact match then we can break out of
the loop. By the end of the loop the block pointer will be pointing to the best free block to return... so we return it.

> 5. You will need to implement Worst Fit.
>
>   - What information do you need to perform a Worst Fit?
>
>   - How would you implement Worst Fit?

Response.

~) We will need to keep track of the size of the largest free space, the block holding that free space, and which block is the tail of the list if you
reach it.
~) We will implement Worst Fit by looping through the list starting at the head. Each iteration will set a pointer equal to the current block so that in
the case no free block is found, this pointer will be the tail of the list.  On the first iteration where a valid free block is found, we will assign
its size to a local variable and set a block pointer equal to it. For each following iteration where a valid free block is found, we will compare it to
our current size variable. If the size of the current block is greater than the variable then we update the variable to the larger size and update the block
pointer to our current block. By the end of this loop we have two pointers where one is either NULL or the "worst" free block, and the other is either the
tail of the list(when the first is NULL) or also the "worst" free block. So in this case we return the first pointer so that it knows to either use the free
block or grow the heap if NULL is returned.

> 6. You will need to implement tracking of different information.
>
>   - What information will you want to track?
>
>   - How will you update these trackers?
>
>   - How will you report these trackers when the program ends?

Response.

~) We will want to keep track of every time the user calls our malloc function and free function and the function doesn't return null. We will want to keep
track of every time we find a free block in our linked-list that we can reuse and then use it, as well as every time we add a new block to our linked list.
We want to keep track of each time we call a split block function after recieving a free block from our list, and each time we call a coalesce function after
freeing a block. A integer should also keep track of the number of blocks the linked-list represents at any given moment. Another integer should keep track of
total size input given to malloc after any ALGIN4 macros are used, while another integer keeps track of the total size represented by all the headers linked
in the list.
~) We will update these trackers by keeping them as global variables that any function can access at any time.
~) We will report this information by using an atexit function. We will pass a function into this that uses dup to reopen stdout. We can do this by calling dup(1) since the exit caused stdout to close and dup will open the lowest filedescriptor. We can then use sprintf to combine the global counters into a char array. We can then call puts() on this array since we reopened stdout.

Demonstration
-------------

> Place a link to your demonstration slides on [Google Drive].
https://docs.google.com/a/nd.edu/presentation/d/1M-5cxrlD4--9RV2s3sahbQpCioq9sYQ7rM_J8X8ZDsM/edit?usp=sharing

Errata
------

> We lack Unit tests and the full extent of the Benchmarking scripts.

Extra Credit
------------

> Describe what extra credit (if any) that you implemented.

[Project 04]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/project04.html
[CSE.30341.FA17]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/
[Google Drive]:     https://docs.google.com/a/nd.edu/presentation/d/1M-5cxrlD4--9RV2s3sahbQpCioq9sYQ7rM_J8X8ZDsM/edit?usp=sharing
