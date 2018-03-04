#include "inode_handler.h"

int inode_init(){
	for (int i = 0; i < MAX_INODE_NB; i++) {
		for (int j = 0; j < MAX_BLOCKS_PER_FILE; j++) {
			inode_table.inode[i].block_id_list[j]=-1;
		}
	}
	return 0;
}


int get_inode() {
	int free_inode,flag = 0;
	for(int i = 0; i < MAX_INODE_NB; i++) {
		if(inode_table.inode[i].block_id_list[0]==-1) {
			flag=1;
			return i;
		}
	}
	if(flag==0)
		return -1;
}

int free_inode(int inode_num) {
	if(inode_num > MAX_INODE_NB - 1 || inode_num < 0) {
		return -1;
	}
	else {
		inode_table.inode[inode_num].block_id_list[0]=-1;
		for (int i = 1; i < MAX_BLOCKS_PER_FILE; i++) {
			inode_table.inode[inode_num].block_id_list[i]=-1;
		}
	}
	return 0;
}

int get_block(int inode_num) {
	return inode_table.inode[inode_num].block_id_list[0];
}

int get_nlink(int inode_num) {
	if(inode_num > MAX_INODE_NB - 1 || inode_num < 0) {
		return -1;
	}
	return inode_table.inode[inode_num].nlink;
}

int set_nlink(int inode_num , int value) {
	if(inode_num > MAX_INODE_NB - 1 || inode_num < 0) {
		return -1;
	}
	inode_table.inode[inode_num].nlink=value;
	return 0;
}

int set_block(int inode_num , int block_id) {
	if(inode_num > MAX_INODE_NB - 1 || inode_num < 0) {
		return -1;
	}
	inode_table.inode[inode_num].block_id_list[0]=block_id;
	return 0;
}