// fs.cpp: File System

#include "sfs/fs.h"

#include <algorithm>
#include <iostream>

#include <assert.h>
#include <stdio.h>
#include <string.h>

// Debug file system -----------------------------------------------------------

void FileSystem::debug(Disk *disk) {
    Block block;

    // Read Superblock
    disk->read(0, block.Data);

    printf("SuperBlock:\n");
	(block.Super.MagicNumber == MAGIC_NUMBER) ? printf("    magic number is valid\n") : printf("    magic number is invalid\n");
    printf("    %u blocks\n"         , block.Super.Blocks);
    printf("    %u inode blocks\n"   , block.Super.InodeBlocks);
    printf("    %u inodes\n"         , block.Super.Inodes);

    // Read Inode blocks
	size_t NodeBlocks = block.Super.InodeBlocks;
	int NodeCounter = 0;
	//~~~~~~~~~~~~~~~~~~~~~//
	// LOOP THROUGH BLOCKS //
	//~~~~~~~~~~~~~~~~~~~~~//
	for (size_t i = 1; i <= NodeBlocks; i++){
		disk->read(i, block.Data);
		//~~~~~~~~~~~~~~~~~~~~~//
		// LOOP THROUGH INODES //
		//~~~~~~~~~~~~~~~~~~~~~//
		for(unsigned int j = 0; j < INODES_PER_BLOCK; j++){
			if (!block.Inodes[j].Valid) continue;
    		printf("Inode %u:\n", j+NodeCounter);
    		printf("    size: %u bytes\n"         , block.Inodes[j].Size);
    		printf("    direct blocks:");
			bool isIndirectBlock = true;
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			// LOOP THROUGH DIRECT POINTERS //
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			for (size_t k = 0; k < POINTERS_PER_INODE; k++){
				if (block.Inodes[j].Direct[k] == 0){
					isIndirectBlock = false;
					break;
				}

				std::cout << " " << block.Inodes[j].Direct[k];
			}
			std::cout << std::endl;
			//~~~~~~~~~~~~~~~~~~~~~//
			// READ INDIRECT BLOCK //
			//~~~~~~~~~~~~~~~~~~~~~//
			if (isIndirectBlock){
				int Indirect = block.Inodes[j].Indirect;
				if (Indirect == 0) continue; // Sanity check
				Block IndirectBlock;
				disk->read(Indirect, IndirectBlock.Data);
    			printf("    indirect block: %u\n", Indirect);
    			printf("    indirect data blocks:");
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				// LOOP THROUGH INDIRECT POINTERS //
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				for (size_t k = 0; k < POINTERS_PER_BLOCK; k++){
					if (IndirectBlock.Pointers[k] != 0) std::cout << " " << IndirectBlock.Pointers[k];
				}
				std::cout << std::endl;
				
				
			}

		}
		NodeCounter += INODES_PER_BLOCK;
	}

}

// Format file system ----------------------------------------------------------

bool FileSystem::format(Disk *disk) {
	if (disk->mounted()) return false;

    // Write superblock
	Block SuperBlock = {0};
	SuperBlock.Super.MagicNumber = MAGIC_NUMBER;
	SuperBlock.Super.Blocks = disk->size();
	SuperBlock.Super.InodeBlocks = SuperBlock.Super.Blocks/10;
	if (SuperBlock.Super.Blocks % 10 != 0) SuperBlock.Super.InodeBlocks++;
	SuperBlock.Super.Inodes = SuperBlock.Super.InodeBlocks * INODES_PER_BLOCK;
	disk->write(0, SuperBlock.Data);

    // Clear all other blocks
	Block EmptyBlock = {0};
	for (unsigned int i = 1; i < disk->size(); i++){
		disk->write(i, EmptyBlock.Data);
	}
    return true;
}

// Mount file system -----------------------------------------------------------

bool FileSystem::mount(Disk *disk) {
	if (isMounted) return false;
    // Read superblock
	Block SuperBlock;

	try {
		disk->read(0, SuperBlock.Data);
	} catch (const std::exception& e){
		return false;
	}

	if (SuperBlock.Super.MagicNumber != MAGIC_NUMBER) return false;

	if (SuperBlock.Super.Blocks != disk->size()) return false;

	unsigned int ExpectedInodeBlocks = SuperBlock.Super.Blocks/10;
	if (SuperBlock.Super.Blocks % 10 != 0) ExpectedInodeBlocks++;
	if (SuperBlock.Super.InodeBlocks != ExpectedInodeBlocks) return false;

	if (SuperBlock.Super.Inodes != ExpectedInodeBlocks * INODES_PER_BLOCK) return false;
    // Set device and mount

	disk->mount();
	isMounted = true;

	// Copy metadata
	MountedDisk = disk;
	TotalBlocks = SuperBlock.Super.Blocks;
	SuperInodeBlocks = SuperBlock.Super.InodeBlocks;
	SuperInodes = SuperBlock.Super.Inodes;

    // Allocate free block bitmap
	FreeBlockBitmap.clear();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// INVALIDATE SUPERBLOCK AND INODE TABLE //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	for (unsigned int i = 0; i <= SuperBlock.Super.InodeBlocks; i++) FreeBlockBitmap.push_back(0);
	//~~~~~~~~~~~~~~~~~~~//
	// FILL BLANK BITMAP //
	//~~~~~~~~~~~~~~~~~~~//
	for (unsigned int i = SuperBlock.Super.InodeBlocks + 1; i < SuperBlock.Super.Blocks; i++) FreeBlockBitmap.push_back(1);
	//~~~~~~~~~~~~~~~~~~~~~//
	// LOOP THROUGH BLOCKS //
	//~~~~~~~~~~~~~~~~~~~~~//
	for (size_t i = 1; i <= SuperBlock.Super.InodeBlocks; i++){
		Block block;
		disk->read(i, block.Data);
		//~~~~~~~~~~~~~~~~~~~~~//
		// LOOP THROUGH INODES //
		//~~~~~~~~~~~~~~~~~~~~~//
		for(unsigned int j = 0; j < INODES_PER_BLOCK; j++){
			if (!block.Inodes[j].Valid) continue;
			bool isIndirectBlock = true;
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			// LOOP THROUGH DIRECT POINTERS //
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			for (size_t k = 0; k < POINTERS_PER_INODE; k++){
				if (block.Inodes[j].Direct[k] == 0){
					isIndirectBlock = false;
					break;
				}
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				// INVALIDATE THE BLOCK IN THE FREE BITMAP //
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				FreeBlockBitmap[block.Inodes[j].Direct[k]] = 0;
			}
			//~~~~~~~~~~~~~~~~~~~~~//
			// READ INDIRECT BLOCK //
			//~~~~~~~~~~~~~~~~~~~~~//
			if (isIndirectBlock){
				int Indirect = block.Inodes[j].Indirect;
				if (Indirect == 0) continue; // Sanity check
				Block IndirectBlock;
				disk->read(Indirect, IndirectBlock.Data);
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				// INVALIDATE THE BLOCK IN THE FREE BITMAP //
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				FreeBlockBitmap[Indirect] = 0;
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				// LOOP THROUGH INDIRECT POINTERS //
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
				for (size_t k = 0; k < POINTERS_PER_BLOCK; k++){
					//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
					// INVALIDATE THE BLOCK IN THE FREE BITMAP //
					//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
					if (IndirectBlock.Pointers[k] != 0) FreeBlockBitmap[IndirectBlock.Pointers[k]] = 0;
				}
			}
		}
	}


    return true;
}

// Create inode ----------------------------------------------------------------

ssize_t FileSystem::create() {
	//~~~~~~~~~~~~~~~~~//
	// CHECK FOR MOUNT //
	//~~~~~~~~~~~~~~~~~//
	if (!isMounted) return -1;


    // Locate free inode in inode table
	//~~~~~~~~~~~~~~~~~~~~~//
	// LOOP THROUGH BLOCKS //
	//~~~~~~~~~~~~~~~~~~~~~//
	int NodeCounter = 0;
	int iNumber = -1;
	unsigned int BlockIndex;
	Block block;
	for (BlockIndex = 1; BlockIndex <= SuperInodeBlocks; BlockIndex++){
		MountedDisk->read(BlockIndex, block.Data);
		//~~~~~~~~~~~~~~~~~~~~~//
		// LOOP THROUGH INODES //
		//~~~~~~~~~~~~~~~~~~~~~//
		for(unsigned int j = 0; j < INODES_PER_BLOCK; j++){
			if (!block.Inodes[j].Valid) {
				//~~~~~~~~~~~~~~~~~~//
				// INITIALIZE INODE //
				//~~~~~~~~~~~~~~~~~~//
				block.Inodes[j].Valid = 1;
				block.Inodes[j].Size = 0;
				iNumber = j + NodeCounter;
		        MountedDisk->write(BlockIndex, block.Data);
                return iNumber;
			}
		}
		NodeCounter += INODES_PER_BLOCK;
	}

    return -1;
}

// Remove inode ----------------------------------------------------------------

bool FileSystem::remove(size_t inumber) {
	if (!isMounted) return false;

    // Load inode information
	if (inumber > SuperInodes - 1) return false; // ex: inumber = 128 but there is only one block of inodes

	int BlockIndex = (inumber / INODES_PER_BLOCK) + 1; // ex: inumber = 128, BlockIndex should be 2
	unsigned int InodeIndex = inumber % INODES_PER_BLOCK; // ex: inumber = 128, InodeIndex should be 0

	Block InodeBlock;
	MountedDisk->read(BlockIndex, InodeBlock.Data);
	if (!InodeBlock.Inodes[InodeIndex].Valid) return false;

    // Free direct blocks
	bool isIndirect = true;
	Block EmptyBlock = {0};
	for (unsigned int i = 0; i < POINTERS_PER_INODE; i++){
		if (InodeBlock.Inodes[InodeIndex].Direct[i] == 0){
			isIndirect = false;
			break;
		}
		FreeBlockBitmap[InodeBlock.Inodes[InodeIndex].Direct[i]] = 1;
		MountedDisk->write(InodeBlock.Inodes[InodeIndex].Direct[i], EmptyBlock.Data);
		InodeBlock.Inodes[InodeIndex].Direct[i] = 0;
	}

    // Free indirect blocks
	if (isIndirect && InodeBlock.Inodes[InodeIndex].Indirect){
		Block IndirectBlock;
		MountedDisk->read(InodeBlock.Inodes[InodeIndex].Indirect, IndirectBlock.Data);
		for (unsigned int i = 0; i < POINTERS_PER_BLOCK; i++){
			if (IndirectBlock.Pointers[i] == 0){
				break;
			}
			FreeBlockBitmap[IndirectBlock.Pointers[i]] = 1;
			MountedDisk->write(IndirectBlock.Pointers[i], EmptyBlock.Data);
			IndirectBlock.Pointers[i] = 0; // Should be unneccessary but you know how life goes

		}
		FreeBlockBitmap[InodeBlock.Inodes[InodeIndex].Indirect] = 1;
		MountedDisk->write(InodeBlock.Inodes[InodeIndex].Indirect, EmptyBlock.Data);
		InodeBlock.Inodes[InodeIndex].Indirect = 0;

	}

    // Clear inode in inode table
	InodeBlock.Inodes[InodeIndex].Valid = 0;
	InodeBlock.Inodes[InodeIndex].Size = 0;
	MountedDisk->write(BlockIndex, InodeBlock.Data);
    return true;
}

// Inode stat ------------------------------------------------------------------

ssize_t FileSystem::stat(size_t inumber) {
	if (!isMounted) return -1;

    // Load inode information
	if (inumber > SuperInodes - 1) return -1;

	int BlockIndex = (inumber / INODES_PER_BLOCK) + 1;
	unsigned int InodeIndex = inumber;
	while (InodeIndex >= INODES_PER_BLOCK) InodeIndex -= INODES_PER_BLOCK;

	Block InodeBlock;
	MountedDisk->read(BlockIndex, InodeBlock.Data);
	if (!InodeBlock.Inodes[InodeIndex].Valid) return -1;

    return InodeBlock.Inodes[InodeIndex].Size;
}

// Read from inode -------------------------------------------------------------

ssize_t FileSystem::read(size_t inumber, char *data, size_t length, size_t offset) {
	if (!isMounted) return -1;

    // Load inode information
	if (inumber > SuperInodes - 1) return -1;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// CALCULATE BLOCK AND INODE INDEX //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	int BlockIndex = (inumber / INODES_PER_BLOCK) + 1;
	unsigned int InodeIndex = inumber % INODES_PER_BLOCK;

	//~~~~~~~~~~~~~~~~~~~~~~~//
	// CHECK FOR VALID INODE //
	//~~~~~~~~~~~~~~~~~~~~~~~//
	Block InodeBlock;
	MountedDisk->read(BlockIndex, InodeBlock.Data);
	if (!InodeBlock.Inodes[InodeIndex].Valid) return -1;
	
	//~~~~~~~~~~~~~~~~~~~~~~//
	// CONFIGURE THE OFFSET //
	//~~~~~~~~~~~~~~~~~~~~~~//
	int OffsetIndex = offset / Disk::BLOCK_SIZE; // Tells what pointer to access. Ex: offset = 4KB OffsetIndex = 1 or second direct pointer
	offset %= Disk::BLOCK_SIZE;					 // Tells what byte to start looking at in the block

    // Adjust length
	bool isIndirect = true;
	int ByteIndex = 0;
	int ReadBytes = 0;
	bool isFirst = true;
	for (unsigned int i = OffsetIndex; i < POINTERS_PER_INODE; i++){
		if (InodeBlock.Inodes[InodeIndex].Direct[i] == 0){
			isIndirect = false;
			break;
		}

		Block DataBlock;
		MountedDisk->read(InodeBlock.Inodes[InodeIndex].Direct[i], DataBlock.Data);
		//~~~~~~~~~~~~~~~~//
		// READ A PORTION //
		//~~~~~~~~~~~~~~~~//
		if (length > Disk::BLOCK_SIZE - offset){
			length -= (Disk::BLOCK_SIZE - offset);
			if (isFirst){
				memcpy(data, DataBlock.Data+offset, Disk::BLOCK_SIZE - offset);
				ReadBytes += Disk::BLOCK_SIZE - offset;
				ByteIndex += Disk::BLOCK_SIZE - offset;
				offset = 0; // Not needed anymore
				isFirst = false;
			}else{
				memcpy(data+ByteIndex, DataBlock.Data, Disk::BLOCK_SIZE);
				ReadBytes += Disk::BLOCK_SIZE;
				ByteIndex += Disk::BLOCK_SIZE;

			}
		//~~~~~~~~~~~~~~~//
		// READ THE REST //
		//~~~~~~~~~~~~~~~//
		}else{
			if (isFirst){
				memcpy(data, DataBlock.Data+offset, length);
				ReadBytes += length;
				offset = 0; // not needed
				isFirst = false;
			}else{
				memcpy(data+ByteIndex, DataBlock.Data, length);
				ReadBytes += length;
			}
			isIndirect = false;
			break;
		}
		
	}

	if (isIndirect && InodeBlock.Inodes[InodeIndex].Indirect){
		Block IndirectBlock;
		MountedDisk->read(InodeBlock.Inodes[InodeIndex].Indirect, IndirectBlock.Data);
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// LOOP THROUGH INDIRECT BLOCKS AND SUBTRACT 5 FROM THE OFFSET INDEX TO LINE UP WITH THE INDIRECT INDEXES //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		for (unsigned int i = OffsetIndex-5; i < POINTERS_PER_BLOCK; i++){
			if (IndirectBlock.Pointers[i] == 0){
				break;
			}

			Block DataBlock;
			MountedDisk->read(IndirectBlock.Pointers[i], DataBlock.Data);
			if (length > Disk::BLOCK_SIZE - offset){
				length -= (Disk::BLOCK_SIZE - offset);
				if (isFirst){
					memcpy(data, DataBlock.Data+offset, Disk::BLOCK_SIZE - offset);
					ReadBytes += Disk::BLOCK_SIZE - offset;
					ByteIndex += Disk::BLOCK_SIZE - offset;
					offset = 0;
					isFirst = false;
				}else{
					memcpy(data+ByteIndex, DataBlock.Data, Disk::BLOCK_SIZE);
					ReadBytes += Disk::BLOCK_SIZE;
					ByteIndex += Disk::BLOCK_SIZE;
	
				}
			}else{
				if (isFirst){
					memcpy(data, DataBlock.Data+offset, length);
					ReadBytes += length;
					offset = 0;
					isFirst = false;
				}else{
					memcpy(data+ByteIndex, DataBlock.Data, length);
					ReadBytes += length;
				}
				break;
			}
			
		}

	}

    // Read block and copy to data
    return ReadBytes;
}

// Write to inode --------------------------------------------------------------

ssize_t FileSystem::write(size_t inumber, char *data, size_t length, size_t offset) {
    // Load inode
	if (!isMounted) return -1;

    // Load inode information
	if (inumber > SuperInodes - 1) return -1;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// CALCULATE BLOCK AND INODE INDEX //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	int BlockIndex = (inumber / INODES_PER_BLOCK) + 1;
	unsigned int InodeIndex = inumber % INODES_PER_BLOCK;

	//~~~~~~~~~~~~~~~~~~~~~~~//
	// CHECK FOR VALID INODE //
	//~~~~~~~~~~~~~~~~~~~~~~~//
	Block InodeBlock;
	MountedDisk->read(BlockIndex, InodeBlock.Data);
	if (!InodeBlock.Inodes[InodeIndex].Valid) return -1;
	
	//~~~~~~~~~~~~~~~~~~~~~~//
	// CONFIGURE THE OFFSET //
	//~~~~~~~~~~~~~~~~~~~~~~//
	int OffsetIndex = offset / Disk::BLOCK_SIZE; // Tells what pointer to access. Ex: offset = 4KB OffsetIndex = 1 or second direct pointer
	offset %= Disk::BLOCK_SIZE;					 // Tells what byte to start looking at in the block
    
    // Write block and copy to data
	bool isIndirect = true;
	int ByteIndex = 0;
	int WriteBytes = 0;
	bool isFirst = true;
	bool isDiskFull = true;
	for (unsigned int i = OffsetIndex; i < POINTERS_PER_INODE; i++){
		unsigned int j;
		for (j = 0; j < TotalBlocks; j++){
			if (FreeBlockBitmap[j]){
				FreeBlockBitmap[j] = 0;
				isDiskFull = false;
				break;
			}
		}
		if (isDiskFull) return WriteBytes;
		InodeBlock.Inodes[InodeIndex].Direct[i] = j;

		//~~~~~~~~~~~~~~~~//
		// READ A PORTION //
		//~~~~~~~~~~~~~~~~//
		Block WriteBlock = {0};
		if (length > Disk::BLOCK_SIZE - offset){
			length -= (Disk::BLOCK_SIZE - offset);
			if (isFirst){
				memcpy(WriteBlock.Data+offset, data, Disk::BLOCK_SIZE - offset);
				WriteBytes += Disk::BLOCK_SIZE - offset;
				ByteIndex += Disk::BLOCK_SIZE - offset;
				offset = 0; // Not needed anymore
				isFirst = false;
			}else{
				memcpy(WriteBlock.Data, data+ByteIndex, Disk::BLOCK_SIZE);
				WriteBytes += Disk::BLOCK_SIZE;
				ByteIndex += Disk::BLOCK_SIZE;

			}
			MountedDisk->write(j, WriteBlock.Data);
		//~~~~~~~~~~~~~~~//
		// READ THE REST //
		//~~~~~~~~~~~~~~~//
		}else{
			if (isFirst){
				memcpy(WriteBlock.Data+offset, data, length);
				WriteBytes += length;
				offset = 0; // not needed
				isFirst = false;
			}else{
				memcpy(WriteBlock.Data, data+ByteIndex, length);
				WriteBytes += length;
			}
			MountedDisk->write(j, WriteBlock.Data);
			isIndirect = false;
			break;
		}
	}

	if (isIndirect){
		unsigned int j;
		for (j = 0; j < TotalBlocks; j++){
			if (FreeBlockBitmap[j]){
				FreeBlockBitmap[j] = 0;
				isDiskFull = false;
				break;
			}
		}
		if (isDiskFull) return WriteBytes;
		InodeBlock.Inodes[InodeIndex].Indirect = j;
		Block IndirectBlock = {0};
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// LOOP THROUGH INDIRECT BLOCKS AND SUBTRACT 5 FROM THE OFFSET INDEX TO LINE UP WITH THE INDIRECT INDEXES //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		for (unsigned int i = OffsetIndex-5; i < POINTERS_PER_BLOCK; i++){
			Block NewBlock = {0};
			unsigned int k;
			for (k = 0; k < TotalBlocks; k++){
				if (FreeBlockBitmap[k]){
					FreeBlockBitmap[k] = 0;
					isDiskFull = false;
					break;
				}
			}
			if (isDiskFull) return WriteBytes;
			IndirectBlock.Pointers[i] = k;

			if (length > Disk::BLOCK_SIZE - offset){
				length -= (Disk::BLOCK_SIZE - offset);
				//~~~~~~~~~~~~~~~//
				// PARTIAL BLOCK //
				//~~~~~~~~~~~~~~~//
				if (isFirst){
					memcpy(NewBlock.Data+offset, data, Disk::BLOCK_SIZE - offset);
					WriteBytes += Disk::BLOCK_SIZE - offset;
					ByteIndex += Disk::BLOCK_SIZE - offset;
					offset = 0;
					isFirst = false;
				//~~~~~~~~~~~~//
				// FULL BLOCK //
				//~~~~~~~~~~~~//
				}else{
					memcpy(NewBlock.Data, data+ByteIndex, Disk::BLOCK_SIZE);
					WriteBytes += Disk::BLOCK_SIZE;
					ByteIndex += Disk::BLOCK_SIZE;
	
				}
				MountedDisk->write(k, NewBlock.Data);
			}else{
				if (isFirst){
					memcpy(NewBlock.Data + offset, data, length);
					WriteBytes += length;
					offset = 0;
					isFirst = false;
				}else{
					memcpy(NewBlock.Data, data + ByteIndex, length);
					WriteBytes += length;
				}
				MountedDisk->write(k, NewBlock.Data);
				break;
			}
			
			
		}
		MountedDisk->write(j, IndirectBlock.Data);
		InodeBlock.Inodes[InodeIndex].Size = WriteBytes;
		MountedDisk->write(BlockIndex, InodeBlock.Data);


	}

    return WriteBytes;
}
