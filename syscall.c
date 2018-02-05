#include "common.h"
#include "syscall.h"
#include "util.h"

directroy_t dir={.block_count=1,.dir_block[0].path="/"};

int sys_mkdir(const char *path, mode_t mode) {
	
	char *dup_path = strdup(path);
	char *p_p=get_parent_path(dup_path);
	//printf("\n\n%s\n\n",p_p);
	int block_count=dir.block_count;
	int i,index=0,flag=0,dir_count;
	for(i=0;i<block_count;i++){
		printf("\n\n%s\n%s\n",dir.dir_block[i].path,dup_path);
		if(strcmp(dir.dir_block[i].path,p_p)==0){

			flag=1;
			index=i;
			dir_count=dir.dir_block[index].dir_count;
			break;
		} 
	}
	if(flag!=1){
	//	printf("WEll i came here");
		fprintf(stderr,"Path doesnt exist");
		return -1;
	}

	if(dir_count>=MAX_DIRENT_NB){
	//	printf("WEll i came here2");
	//	printf("No space left\n");
		return -1;
	}
	for(i=0;i<=dir_count;i++){
		if(strcmp(basename(dup_path),dir.dir_block[index].dir_ent[i].name)==0){
		//	printf("WEll i came here3");
		//	fprintf(stderr," Directory exists your retard");
			return -1;
		}

	}

	strcpy(dir.dir_block[index+1].path,dup_path);
	dir.block_count+=1;
	strcpy(dir.dir_block[index].dir_ent[dir_count].name,basename(dup_path));

	//printf("I CAME HERE %s %d",path,dir_count);
	dir.dir_block[index].dir_count+=1;
	return 0;
}

int sys_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset) {
	printf("\n Reading path %s\n",strdup(path));
	char *dup_path = strdup(path);
	char *p_p=get_parent_path(dup_path);
	int i,index=0,flag=0,dir_count=0;
	filler( buf, ".", NULL, 0 ); // Current Directory
	filler( buf, "..", NULL, 0 ); // Parent Directory
	for(i=0;i<dir.block_count;i++){
		//printf("\n%d\n",i);
		if(strcmp(dir.dir_block[i].path,dup_path)==0){
			flag=1;
			index=i;
			dir_count=dir.dir_block[index].dir_count;

			break;
		} 
	}
	//printf("Flag is %d\t Index is %d\n\n",flag,index);
	if(flag!=1){
		fprintf(stderr,"Path doesnt exist");
		return -1;
	}

	for(i=0;i<dir_count;i++){

		filler(buf, dir.dir_block[index].dir_ent[i].name, NULL, 0);
	}
	free(dup_path);
	

	return 0;
}

int sys_lstat(const char *path, struct stat *stbuf) {

	int res = 0;

	
	char *dup_path = strdup(path);

	int i,dir_count=0,index,flag=0;

	char *p_p=get_parent_path(dup_path);

	int block_count=dir.block_count;

	for(i=0;i<block_count;i++){

		if(strcmp(dir.dir_block[i].path,dup_path)==0){

			flag=1;
			index=i;
			dir_count=dir.dir_block[index].dir_count;
			break;

		} 
	}
	stbuf->st_mode = S_IFDIR | 0755;
	stbuf->st_nlink = dir_count+2;

	if(flag==0){
	
		res =-ENOENT;
	}
	free(dup_path);
	return res;
}

