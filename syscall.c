#include "common.h"
#include "syscall.h"

static struct dir_field dirent[MAX_DIRENT_NB];

static int num_active_dirent = 2;

int sys_mkdir(const char *path, mode_t mode) {
	// TODO: implement mkdir	

	return 0;
}

int sys_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset) {
	// TODO: implement readdir

	return 0;
}

int sys_lstat(const char *path, struct stat *stbuf) {
	int res = -ENOENT;

	char *dup_path = strdup(path);

	int i;
	for (i = 2; i < NUM_DIRENT; ++i) {
		if (dirent[i].inode_id == 0) {
			break;
		}
		if (strcmp(basename(dup_path), dirent[i].name) == 0) {
 			stbuf->st_mode = S_IFDIR | 0755;
			stbuf->st_nlink = 2;
			res = 0;
			break;
		}	
	}

	if (strcmp(path, "/") == 0) {
		res = 0;
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = num_active_dirent;
	}

	free(dup_path);

	return res;
}

