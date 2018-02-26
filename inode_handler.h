#include "common.h"

static struct inode_table_t inode_table ;

int inode_init();
int get_inode();
int free_inode(int inode_num);
int get_block(int inode_num);
int get_nlink(int inode_num);
int set_nlink(int inode_num , int value);