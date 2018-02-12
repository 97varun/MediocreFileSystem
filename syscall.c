#include "common.h"
#include "syscall.h"
#include "util.h"

static struct directory_t dir;

int sys_init() {
	int free_idx = 0;
	strcpy(dir.dir_block[free_idx].path, "/");
	fill_dir_ent(&(dir.dir_block[free_idx]), ".");
	fill_dir_ent(&(dir.dir_block[free_idx]), "..");
	dir.dir_block[free_idx].num_ent = 2;
	return 0;
}

int sys_mkdir(const char *path, mode_t mode) {
	char *name = strdup(path);
	basename(name);

	int free_idx = 0;
	for (; free_idx < MAX_DIR_LEN; ++free_idx) {
		// empty path => free entry
		if (!strlen(dir.dir_block[free_idx].path)) {
			break;
		}
	}

	if (free_idx >= MAX_DIR_LEN) {
		printf("path not found\n");
		return 1;
	}
	
	strcpy(dir.dir_block[free_idx].path, path);
	dir.dir_block[free_idx].num_ent = 2;

	// initialise directory with '.' and '..'
	fill_dir_ent(&(dir.dir_block[free_idx]), ".");
	fill_dir_ent(&(dir.dir_block[free_idx]), "..");
	
	// find parent directory
	char *dup_path = strdup(path);
	char *par_path = dirname(dup_path);
	int par_idx;
	for (par_idx = 0; par_idx < MAX_DIR_LEN; ++par_idx) {
		if (!(strcmp(par_path, dir.dir_block[par_idx].path))) {
			break;
		}	
	}

	// add entry to parent directory
	fill_dir_ent(&(dir.dir_block[par_idx]), name);
	// increase hardlink count
	dir.dir_block[par_idx].num_ent++;

	free(name);
	free(par_path);

	return 0;
}

int sys_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset) {
	int flag = 0;
	for(int i = 0; i < MAX_DIR_LEN; i++) {
		if(strcmp(path, dir.dir_block[i].path) == 0) {
			flag = 1;
			filler(buf, ".", NULL, 0);
			filler(buf, "..", NULL, 0);
			
			for (int j = 0; j < MAX_DIRENT_NB; j++) {
				if(strlen(dir.dir_block[i].dir_ent[j].name)) {
					char *name = basename(dir.dir_block[i].dir_ent[j].name);
					filler(buf, name, NULL, 0);
				}

			}
			break;
		}
	}
	// if no dir with given path is there
	if(!flag) {
		return -1;
	}

	return 0;
}

int sys_open(const char *path, mode_t mode) {
	int flag1=0,flag2=0,fd;
	char * dup_path = strdup(path);
	char *par_path = dirname(dup_path);
	char *file_name = basename(dup_path);
	for (int i = 0; i < MAX_DIR_LEN ; i++) {
		if (strcmp (par_path , dir.dir_block[i].path) == 0) {
			flag = 1;
			for (int j = 0; j < MAX_DIRENT_NB ; j++) {
				if(strcmp (file_name,dir.dir_block[i].dir_ent[j].name) == 0):
					fd = dir.dir_block[i].dir_ent[j].inode_id;
			}
		}
	}
	if(flag1 && flag2)
		return fd;
	return -1;

}

int sys_lstat(const char *path, struct stat *stbuf) {
	int res = -ENOENT;

	// all exisiting entries are directories for now.
	int i;
	for (int i = 0; i < MAX_DIR_LEN; ++i) {
		if (!(strcmp(path, dir.dir_block[i].path))) {
			stbuf->st_mode = S_IFDIR | 0777;
			stbuf->st_nlink = dir.dir_block[i].num_ent;
			res = 0;
			break;
		}		
	}

	return res;
}

