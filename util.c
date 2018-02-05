#include "common.h"

int fill_dir_ent(struct dir_block_t *dir_block, char *entry) {
	int i;
	for (i = 0; i < MAX_DIRENT_NB; ++i) {
		// empty entry => free
		if (!(strlen(dir_block->dir_ent[i].name))) {
			break;
		}
	}
	if (i >= MAX_DIRENT_NB) {
		return 1;
	}
	strcpy(dir_block->dir_ent[i].name, entry);
	return 0;
}
