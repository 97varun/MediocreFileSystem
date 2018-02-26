#include "common.h"
#include "syscall.h"
#include "util.h"

static struct directory_t dir;
static struct fd_table_t fd_table; //file descriptor

int sys_init() {
	int free_idx = 0;
	strcpy(dir.dir_block[free_idx].path, "/");
	fill_dir_ent(&(dir.dir_block[free_idx]), ".", DIR);
	fill_dir_ent(&(dir.dir_block[free_idx]), "..", DIR);
	dir.dir_block[free_idx].num_ent = 2;
	
	// initializing fd_table
	for (int i = 0; i < MAX_FDT_LEN; i++) {
		fd_table.file_desc[i].fd = -1;
		fd_table.file_desc[i].inode_id = -1;
	}
	
	inode_init();

	return 0;
}

int sys_mkdir(const char *path, mode_t mode) {
	char *dup_path = strdup(path);
	char *name = strdup(basename(dup_path));
	char *par_path = strdup(dirname(dup_path));

	int free_idx = 0;
	for (; free_idx < MAX_DIR_LEN; ++free_idx) {
		// empty path => free entry
		if (!strlen(dir.dir_block[free_idx].path)) {
			break;
		}
	}

	if (free_idx >= MAX_DIR_LEN) {
		printf("not enough space\n");
		return 1;
	}
	
	// add path
	strcpy(dir.dir_block[free_idx].path, path);
	dir.dir_block[free_idx].num_ent = 2;

	// initialise directory with '.' and '..'
	fill_dir_ent(&(dir.dir_block[free_idx]), ".", DIR);
	fill_dir_ent(&(dir.dir_block[free_idx]), "..", DIR);
	
	// find parent directory
	int par_idx;
	for (par_idx = 0; par_idx < MAX_DIR_LEN; ++par_idx) {
		if (!(strcmp(par_path, dir.dir_block[par_idx].path))) {
			break;
		}	
	}

	// add entry to parent directory
	fill_dir_ent(&(dir.dir_block[par_idx]), name, DIR);

	// increase hardlink count
	dir.dir_block[par_idx].num_ent++;

	free(name);
	free(par_path);
	free(dup_path);
	
	return 0;
}

int sys_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset) {
	int flag = 0;
	for(int i = 0; i < MAX_DIR_LEN; i++) {
		if(strcmp(path, dir.dir_block[i].path) == 0) {
			flag = 1;
			
			for (int j = 0; j < MAX_DIRENT_NB; j++) {
				if(strlen(dir.dir_block[i].dir_ent[j].name)) {
					filler(buf, dir.dir_block[i].dir_ent[j].name, NULL, 0);
				}

			}
			
			break;
		}
	}
	// if no dir with given path is there
	if(!flag) {
		printf("here\n");
		return -1;
	}

	return 0;
}

int sys_open(const char *path, mode_t mode) {
	int flag1=0,flag2=0,flag3=0;
	int fd,myInode;
	char * dup_path = strdup(path);
	char *par_path = dirname(dup_path);
	char *file_name = basename(dup_path);
	//Finding inode of file
	for (int i = 0; i < MAX_DIR_LEN ; i++) {
		if (strcmp (par_path , dir.dir_block[i].path) == 0) {
			flag1 = 1;
			for (int j = 0; j < MAX_DIRENT_NB ; j++) {
				if(strcmp (file_name,dir.dir_block[i].dir_ent[j].name) == 0) {
					myInode = dir.dir_block[i].dir_ent[j].inode_id;
				}
			}
		}
	}
	//Finding smallest free fd
	for (int i = 0 ; i < MAX_FDT_LEN ; i++) {
		if(fd_table.file_desc[i].fd = -1) {
			fd_table.file_desc[i].fd = i;
			fd_table.file_desc[i].inode_id = myInode;
			flag3 = 1;
			fd = i;
			break;	
		}
	}

	if(flag1 && flag2 && flag3) {
		//printf("fd : %d\n", &fd);
		return fd;
	}

	return -1;
}

int sys_close(int fd) {
	//if file is already closed 
	if(fd_table.file_desc[fd].fd==-1) {
		return -1;
	}
	else {
		fd_table.file_desc[fd].fd=-1;
		fd_table.file_desc[fd].inode_id=-1;
	}	
	return 0;
}

int sys_lstat(const char *path, struct stat *stbuf) {
	char *dup_path = strdup(path);
	char *name = strdup(basename(dup_path));
	char *par_path = strdup(dirname(dup_path));
	
	if (!strcmp(path, "/")) {
		int dir_idx = 0;
		stbuf->st_mode = S_IFDIR | 0777;
		stbuf->st_nlink = dir.dir_block[dir_idx].num_ent;
		return 0;
	}
	
	// find parent in directory
	int par_idx;
	for (par_idx = 0; par_idx < MAX_DIR_LEN; ++par_idx) {
		if (!(strcmp(par_path, dir.dir_block[par_idx].path))) {
			break;
		}	
	}
	
	// path does not exist
	if (par_idx >= MAX_DIR_LEN) {
		return -ENOENT;
	}
	
	if (name == NULL || par_path == NULL) {
		return -ENOENT;
	}
	
	// find entry in parent
	int j;
	for (j = 0; j < MAX_DIRENT_NB; ++j) {
		if (!(strcmp(name, dir.dir_block[par_idx].dir_ent[j].name))) {
			break;
		}
	}

	// file or directory does not exist
	if (j >= MAX_DIRENT_NB) {
		return -ENOENT;
	}

	// determine type, file or directory
	if (dir.dir_block[par_idx].dir_ent[j].type == REG_FILE) {
		stbuf->st_mode = S_IFREG | 0777;
	} else {
		int dir_idx;
		for (dir_idx = 0; dir_idx < MAX_DIR_LEN; ++dir_idx) {
			if (!strcmp(path, dir.dir_block[dir_idx].path)) {
				break;
			}
		}
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = dir.dir_block[dir_idx].num_ent;
	}

	return 0;
}

int sys_rmdir(const char *path) {
	char *dup_path = strdup(path);
	char *name = strdup(basename(dup_path));
	char *par_path = strdup(dirname(dup_path));

	// find required path
	int i;
	for (i = 0; i < MAX_DIR_LEN; ++i) {
		if (!strcmp(path, dir.dir_block[i].path)) {
			break;
		}		
	}
	
	// path does not exist
	if (i == MAX_DIR_LEN) {
		return -ENOENT;
	}
	
	// make path empty
	dir.dir_block[i].path[0] = '\0';
	dir.dir_block[i].num_ent = 0;
	
	// find parent in directory
	for (i = 0; i < MAX_DIR_LEN; ++i) {
		if (!strcmp(par_path, dir.dir_block[i].path)) {
			break;
		}	
	}
	
	// find entry in parent
	int j;
	for (j = 0; j < MAX_DIRENT_NB; ++j) {
		if (!strcmp(name, dir.dir_block[i].dir_ent[j].name)) {
			break;
		}
	}
	
	// make entry empty
	dir.dir_block[i].dir_ent[j].name[0] = '\0';
	dir.dir_block[i].num_ent--;
	
	free(name);
	free(par_path);
	free(dup_path);
	
	return 0;
}

int sys_mknod(const char *path) {
	char *dup_path = strdup(path);
	char *name = strdup(basename(dup_path));
	char *par_path = strdup(dirname(dup_path));
	
	// find parent directory
	int par_idx;
	for (par_idx = 0; par_idx < MAX_DIR_LEN; ++par_idx) {
		if (!(strcmp(par_path, dir.dir_block[par_idx].path))) {
			break;
		}	
	}

	// add entry to parent directory
	fill_dir_ent(&(dir.dir_block[par_idx]), name, REG_FILE);

	// increase hardlink count
	dir.dir_block[par_idx].num_ent++;
	
	free(name);
	free(par_path);
	free(dup_path);
		
	return 0;
}

