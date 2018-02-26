#include "common.h"
#include "disk.h"

static char **block;
int *bitmap; // if 1 write possible, no otherwise

// initialise disk
int init_disk() {
	block = malloc(MAX_BLOCKS * sizeof(char*));
	bitmap = malloc(MAX_BLOCKS * sizeof(int));
	memset(bitmap, 1, sizeof(bitmap));
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
	return 0;
}

// read block, given block id
int read_block(int block_id, void *buf) {
	if (block_id < MAX_BLOCKS) {
		memcpy(buf, block[block_id], BLOCK_SZ);
		return 0;
	}
	return -1;
}

// write a block
int write_block(void *buf) {
	int block_id;
	for (block_id = 0; block_id < MAX_BLOCKS; ++block_id) {
		if (bitmap[block_id]) {
			break;
		}
	}
	if (block_id < MAX_BLOCKS) {
		memcpy(block[block_id], buf, BLOCK_SZ);
		bitmap[block_id] = 0;
		return 0;
	}
	return -1;
}

// erase a block
int erase_block(int block_id) {
	if (block_id >= 0 && block_id < MAX_BLOCKS) {
		bitmap[block_id] = 1;
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
