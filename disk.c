#include "common.h"
#include "disk.h"

static char **block;
int *bitmap; // if 1 write possible, no otherwise

// initialise disk
int init_disk() {
	block = malloc(MAX_BLOCKS * sizeof(char*));
	bitmap = malloc(MAX_BLOCKS * sizeof(int));
	
	
	int j;
	for (j = 0; j < MAX_BLOCKS; ++j) {
		bitmap[j] = 1;
	}
	
	
	
	if (!block) {
		return -1;
	}
	int i;
	for (i = 0; i < MAX_BLOCKS; ++i) {
		block[i] = malloc(BLOCK_SZ * sizeof(char));
		if (!block[i]) {
			return -1;
		}
	}
	
	FILE* fp = fopen("/tmp/medfs_disk", "r+");
	for (i = 0; i < MAX_BLOCKS; ++i) {
		fread(block[i], 1, BLOCK_SZ, fp);	
	}
	fclose(fp);
	
	fp = fopen("/tmp/medfs_bitmap", "r+");
	fread(bitmap, sizeof(int), MAX_BLOCKS, fp);
	fclose(fp);
	
	printf("bitmap\n");
	for (i = 0; i < 10; ++i) {
		printf("%d", bitmap[i]);
	}
	printf("\n");
	
	printf("first block: %s\n", block[0]);
	
	return 0;
}

// read block, given block id
int read_block(int block_id, void *buf) {
	if (block_id < MAX_BLOCKS) {
		memcpy(buf, block[block_id], BLOCK_SZ);
		printf("block_id: %d\n", block_id);
		printf("block[block_id] :%s\n", block[block_id]);
		return 0;
	}
	return -1;
}

// write a block
int write_block(const void *buf) {
	int block_id;
	for (block_id = 2; block_id < MAX_BLOCKS; ++block_id) {
		if (bitmap[block_id]) {
			break;
		}
	}
	if (block_id < MAX_BLOCKS) {
		memcpy(block[block_id], buf, BLOCK_SZ);
		bitmap[block_id] = 0;
		
		FILE* fp = fopen("/tmp/medfs_disk", "w+");
		int i;
		for (i = 0; i < MAX_BLOCKS; ++i) {
			int n = fwrite(block[i], 1, BLOCK_SZ, fp);
		}
		fclose(fp);
		fp = fopen("/tmp/medfs_bitmap", "w+");
		int n = fwrite(bitmap, sizeof(int), MAX_BLOCKS, fp);
		fclose(fp);
		
		return block_id;
	}
	
	return -1;
}

// write block at a particular block id
int write_block_at(int block_id, const void *buf) {
	if (block_id < MAX_BLOCKS) {
		memcpy(block[block_id], buf, BLOCK_SZ);
		bitmap[block_id] = 0;
		
		FILE* fp = fopen("/tmp/medfs_disk", "w+");
		int i;
		for (i = 0; i < MAX_BLOCKS; ++i) {
			int n = fwrite(block[i], 1, BLOCK_SZ, fp);
		}
		fclose(fp);
		fp = fopen("/tmp/medfs_bitmap", "w+");
		int n = fwrite(bitmap, sizeof(int), MAX_BLOCKS, fp);
		fclose(fp);
		
		return 0;
	}
	
	return -1;
}

// erase a block
int erase_block(int block_id) {
	if (block_id >= 0 && block_id < MAX_BLOCKS) {
		bitmap[block_id] = 1;
		FILE* fp = fopen("/tmp/medfs_bitmap", "w+");
		fwrite(block, sizeof(int), MAX_BLOCKS, fp);
		fclose(fp);
		
		return 0;
	}
	
	return -1;
}

void free_disk() {
	int i;
	for (i = 0; i < MAX_BLOCKS; ++i) {
		free(block[i]);
		block[i] = NULL;
	}
	free(block);
	block = NULL;
}
