#ifndef SYSCALL_H
#define SYSCALL_H

#include "common.h"

int sys_mkdir(const char *path, mode_t mode);
int sys_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset);
int sys_lstat(const char *path, struct stat *stbuf);
int sys_open(const char *path, mode_t mode);
int sys_init();

#endif
