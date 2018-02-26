#include "common.h"
#include "disk.h"

static char **block;

// initialise disk
int init_disk() {
	block = malloc(MAX_BLOCKS * sizeof(char*));
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

// write a block, given block_id
int write_block(int block_id, void *buf) {
	if (block_id < MAX_BLOCKS) {
		memcpy(block[block_id], buf, BLOCK_SZ);
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
