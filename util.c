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

// please free dup_path in the caller to avoid memory leaks
char *get_parent_path(const char *path) {
	char *dup_path = strdup(path);
	for (int i = strlen(dup_path) - 1; i; --i) {
		if (dup_path[i] != '/') {
			dup_path[i] = '\0';		
		} else {
			dup_path[i] = '\0';
			break;
		}	
	}
	return dup_path;
}
