#ifndef COMMON_H
#define COMMON_H

#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <libgen.h>

#define MAX_DIRENT_NB 10
#define MAX_NAME_LEN 64
#define MAX_BLOCKS_PER_FILE 10
#define MAX_PATH_LEN 64
#define MAX_DIR_LEN 100
#define MAX_INODE_NB 100
#define MAX_FDT_LEN 100

enum type_t {
	DIR, REG_FILE
};

struct inode_t {
	int inode_id;
	int block_id_list[MAX_BLOCKS_PER_FILE];
	mode_t mode;
	nlink_t nlink;
};

struct dir_ent_t {
	char name[MAX_NAME_LEN];
	enum type_t type;
	int inode_id;
};

struct dir_block_t {
	char path[MAX_PATH_LEN];
	struct dir_ent_t dir_ent[MAX_DIRENT_NB];
	int num_ent;
};

struct directory_t {
	struct dir_block_t dir_block[MAX_DIR_LEN];
};

struct inode_table_t {
	struct inode_t inode[MAX_INODE_NB];
};

struct file_desc_t {
	int fd;
	int inode_id;
};

struct fd_table {
	struct file_desc_t file_desc[MAX_FDT_LEN];
};

#endif
