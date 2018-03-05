#include "common.h"
#include "syscall.h"

static void *medfs_init(struct fuse_conn_info *conn) {
	(void) conn;
	printf("init called\n");
	sys_init();
	return NULL;
}


static int medfs_getattr(const char *path, struct stat *stbuf) {
	printf("getattr called, path: %s\n", path);
	return sys_lstat(path, stbuf);
}

static int medfs_mkdir(const char *path, mode_t mode) {
	printf("mkdir called, path: %s\n", path);
	return sys_mkdir(path, mode);
}

static int medfs_mknod(const char *path, mode_t mode, dev_t dev) {
	printf("mknod called, path: %s\n", path);
	return sys_mknod(path);
}

static int medfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	printf("readdir called, path: %s\n", path);
	return sys_readdir(path, buf, filler, offset);
}

static int medfs_rmdir(const char *path) {
	printf("rmdir called, path: %s\n", path);
	return sys_rmdir(path);
}

static int medfs_release(const char *path, struct fuse_file_info *fi) {
	printf("close called, path: %s\n", path);
	return sys_close(fi->fh);
}

static int medfs_open(const char *path, struct fuse_file_info *fi) {
	printf("open called, path: %s\n", path);
	return sys_open(path, fi->flags);
}

static int medfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	printf("read called, path: %s\n", path);
	return sys_pread(fi->fh, buf, size, offset);
}

static int medfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	printf("write called, path: %s\n", path);
	return sys_pwrite(fi->fh, buf, size, offset);
}

static int medfs_unlink(const char *path) {
	printf("unlink called, path: %s\n", path);
	return sys_unlink(path);
}

static struct fuse_operations medfs_oper = {
	.init		= medfs_init,
	.getattr	= medfs_getattr,
	.mkdir		= medfs_mkdir,
	.mknod		= medfs_mknod,
	.readdir	= medfs_readdir,
	.open		= medfs_open,
	.read		= medfs_read,
	.write		= medfs_write,
	.release	= medfs_release,
	.rmdir		= medfs_rmdir,
	.unlink		= medfs_unlink,
};

int main(int argc, char *argv[]) {
	printf("File system mounted!\n");
	return fuse_main(argc, argv, &medfs_oper, NULL);	
}
