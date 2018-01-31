CSE.30341.FA17: Project 06
==========================

This is the documentation for [Project 06] of [CSE.30341.FA17].

Members
-------

1. Zac Cendejas (zcendeja@nd.edu)
2. Will Fritz (wfritz@nd.edu)

Design
------

> 1. To implement `Filesystem::debug`, you will need to load the file system
>    data structures and report the **superblock** and **inodes**.
>
>       - How will you read the superblock?
>       - How will you traverse all the inodes?
>       - How will you determine all the information related to an inode?
>       - How will you determine all the blocks related to an inode?

Response.

~ You can read the superblock by calling read() on the Disk object and passing in a 0 and the data part of a block union.
~ You would loop through the total InodeBlocks by reading from the disk and then loop through each inode in the array on the block on each iteration.
~ You can determine the information related to an inode by using the inumber as the index of the array of inodes found in a block. You can then
  use this to access the struct information of the inode relating to that index.
~ You can determine all the blocks related to an inode by looping through the Direct array until you reach a value of zero (Meaning no pointer).
  If you loop through the whole array then you can check if the Indirect field is non-zero and if so, read its block from the disk and loop through
  the array of pointers until a zero is found.

> 2. To implement `FileSystem::format`, you will need to write the superblock
>    and clear the remaining blocks in the file system.
>
>       - What pre-condition must be true before this operation can succeed?
>       - What information must be written into the superblock?
>       - How would you clear all the remaining blocks?

Response.

~ Disk->mounted() must not return true.
~ The superblock needs the MagicNumber written, the total blocks (gotten from Disk.size()), use 10% of the blocks to get the InodeBlocks, multiply this
  by INODES_PER_BLOCK to get the total inodes.
~ You could use memset to create an empty block. You would then write that block to each block on the disk to clear it.

> 3. To implement `FileSystem::mount`, you will need to prepare a filesystem
>    for use by reading the superblock and allocating the free block bitmap.
>
>       - What pre-condition must be true before this operation can succeed?
>       - What sanity checks must you perform?
>       - How will you record that you mounted a disk?
>       - How will you determine which blocks are free?

Response.

~ You must be able to read index 0 of the disk without catching an error. (Do a try catch)
~ The magic number must match the macro and the inodeblocks must be 10% of the total.
~ You will set the disk pointer equal to a global disk pointer and call disk->mount().
~ Start with a full free block bitmap, then loop through the inodes and loop through each valid node to find the blocks in use. Invalidate these blocks from
  the free block bitmap.

> 4. To implement `FileSystem::create`, you will need to locate a free inode
>    and save a new inode into the inode table.
>
>       - How will you locate a free inode?
>       - What information would you see in a new inode?
>       - How will you record this new inode?

Response.

~ You would loop through the inodes array of the inode blocks by reading from the disk until an invalid inode was found.
~ You could see nothing or you could see garbage irl.
~ You would create a new Block.inode with the valid bit of the found inode set to 1 and every other element cleared to zero. You would then write this block
  back to the space on the disk it was read from.

> 5. To implement `FileSystem::remove`, you will need to locate the inode and
>    then free its associated blocks.
>
>       - How will you determine if the specified inode is valid?
>       - How will you free the direct blocks?
>       - How will you free the indirect blocks?
>       - How will you update the inode table?

Response.

~ Read the block it should be on and use the inumber to index the inode. Check the valid bit for a 1. Be sure the iNumber isn't too large or too small.
~ Write an empty block to their location on disk and validate them in the free block bitmap.
~ Read indirect block then, loop through the indirect blocks and write and empty block to each one and validate in the free block bitmap. Then write an empty
  block to the indirect block and validate that block in the free block bitmap.
~ Write back to the block you read from with a new array of inodes.

> 6. To implement `FileSystem::stat`, you will need to locate the inode and
>    return its size.
>
>       - How will you determine if the specified inode is valid?
>       - How will you determine the inode's size?

Response.

~ Read the block it should be on and use the inumber to index the inode. Check the valid bit for a 1.
~ Read the size from the indexed inode.

> 7. To implement `FileSystem::read`, you will need to locate the inode and
>    copy data from appropriate blocks to the user-specified data buffer.
>
>       - How will you determine if the specified inode is valid?
>       - How will you determine which block to read from?
>       - How will you handle the offset?
>       - How will you copy from a block to the data buffer?

Response.

~ The valid bit of the node must be set and the offset must be in the bounds of the direct array or indirect array and not point to zero.
~ You will use the offset to calculate an index. If the result is less than 5 then you can use the result to decide which direct block to 
  start reading from. Otherwise you need to subtract 5 from the result and read in the indirect block. You can use this result to index the 
  indirect block and decide where to start reading.
~ Be sure it isn't less than 0 or greater than INODES_PER_BLOCK + 4.
~ You would push the data of each block to a vector. Once you finish reading the data blocks you can use memcpy and the size read to copy the vector to 
  the data buffer.

> 8. To implement `FileSystem::write`, you will need to locate the inode and
>    copy data the user-specified data buffer to data blocks in the file
>    system.
>
>       - How will you determine if the specified inode is valid?
>       - How will you determine which block to write to?
>       - How will you handle the offset?
>       - How will you know if you need a new block?
>       - How will you manage allocating a new block if you need another one?
>       - How will you copy from a block to the data buffer? (...switched?)
>       - How will you update the inode?

Response.

~ The valid bit of the node must be set and the offset must be in the bounds of the direct array or indirect array and not point to zero.
~ You will use the offset to calculate an index. If the result is less than 5 then you can use the result to decide which direct block to 
  start reading from. Otherwise you need to subtract 5 from the result and read in the indirect block. You can use this result to index the 
  indirect block and decide where to start reading.
~ Be sure it isn't less than 0 or greater than INODES_PER_BLOCK + 4.
~ Keep track of how much has been written and if it is less than the Length then another block is needed.
~ Search the free list for a free block. Invalidate it. Write to it. Make a pointer to it in the inode.
~ Use memcpy to loop through the data buffer 4KB at a time while copying that data to an array. That data can then be written to a block.
~ Update the pointers as blocks are allocated. Use a counter to know when the direct pointers are full then allocate a block for the indirect pointer.

Errata
------

> Describe any known errors, bugs, or deviations from the requirements.
~ Either Read(), Write(), or both have an issue causing them not to transfer data correctly.
~ Remove test fails because of Read()/Write() issue.


Extra Credit
------------

> Describe what extra credit (if any) that you implemented.
~ A kind heart with a desire to learn

[Project 06]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/project06.html
[CSE.30341.FA17]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/
[Google Drive]:     https://drive.google.com
