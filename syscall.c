#include "common.h"
#include "syscall.h"
#include "util.h"

struct directory_t dir;

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

	// initialise directory with '.' and '..'
	fill_dir_ent(&(dir.dir_block[free_idx]), ".");
	fill_dir_ent(&(dir.dir_block[free_idx]), "..");
	
	// find parent directory
	char *par_path = get_parent_path(path);
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
		//if dir with given path as prefix is found
		if( strlen(dir.dir_block[i].path)>strlen(path) ){
			int c=0;
			for(int j=0; j < strlen(path) ; j++){ 
				if(path[j]==dir.dir_block[i].path[j]) 
					c++;	
			}

			//if it has same prefix as path
			if(c==strlen(path)) {
				int j=strlen(path)+1;
				
				//getting the name till "/"
				while(dir.dir_block[i][j]!='\0')
					j++;
				
				// storing it char array
				char name[j-strlen(path)-1];
				for(int k=strlen(path)+1;k<j;k++) {
					name[k-strlen(path)-1]=dir.dir_block[i][j];
				}
				

				//filler
				filler(buf, name ,NULL,0,0);	
			}
		}
		//if dir with given path is found adding "." and ".." 
		elif(strcmp(path,dir.dir_block[i].path == 0)) {
			filler(buf, ".", NULL, 0, 0);
			filler(buf, "..", NULL, 0, 0);
			flag=1;
		}
	}
	// if no dir with given path is there
	if(!flag):
		return -1

	return 0;
}

int sys_lstat(const char *path, struct stat *stbuf) {
	int res = -ENOENT;

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

