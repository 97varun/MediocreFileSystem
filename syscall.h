#ifndef SYSCALL_H
#define SYSCALL_H

#include "common.h"

int sys_mkdir(const char *path, mode_t mode);
int sys_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset);
int sys_lstat(const char *path, struct stat *stbuf);
int sys_pwrite(int fildes, const void *buf, size_t nbyte, off_t offset);
int sys_pread(int fildes, void *buf, size_t nbyte, off_t offset);
int sys_open(const char *path, int flags);
int sys_close(int fd);
int sys_init();
int sys_rmdir(const char *path);
int sys_mknod(const char *path);
int sys_unlink(const char *path);

#endif
