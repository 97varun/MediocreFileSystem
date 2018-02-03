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
#define MAX_NAME_LEN 10
#define MAX_BLOCKS_PER_FILE 10

struct inode {
	int inode_id;
	int block_id_list[MAX_BLOCKS_PER_FILE];
	mode_t mode;
	nlink_t nlink;
}

struct directroy {

}

#endif
