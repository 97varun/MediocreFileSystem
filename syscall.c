#include "common.h"
#include "syscall.h"
#include <string.h>

static struct directroy_t dir;

int sys_mkdir(const char *path, mode_t mode) {
	// TODO: implement mkdir	

	return 0;
}

int sys_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset) {
	int flag=0;
	for(int i=0; i<MAX_DIR_LEN ; i++) {
		//if dir with given path as prefix is found
		/*
		if( strcmp(path,dir.dir_block[i].path ) ){
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
		*/
		if(strcmp(path,dir.dir_block[i].path) == 0) {
			flag=1;
			filler(buf, ".", NULL, 0, 0);
			filler(buf, "..", NULL, 0, 0);
			
			char *name;
			for (int j = 0; j < MAX_DIRENT_NB ; j++) {
				name = dir.dir_block[i].dir_ent[j].name;
				if(strlen(name)!=0)
					filler(buf, name ,NULL ,0 ,0);

			}
			
		}
	}
	// if no dir with given path is there
	if(flag==0):
		return -1

	return 0;
}

int sys_lstat(const char *path, struct stat *stbuf) {
	int res = -ENOENT;

	/* just for reference
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
	*/

	return res;
}

