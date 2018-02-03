#include "common.h"
#include "syscall.h"

static int fs_getattr(const char *path, struct stat *stbuf) {
	printf("getattr called, path: %s\n", path);

	return sys_lstat(path, stbuf);
}

static int fs_mkdir(const char *path, mode_t mode) {
	printf("mkdir called\n");
	sys_mkdir(path, mode);
	return 0;
}

static int fs_mknod(const char *path, mode_t mode, dev_t dev) {
	printf("mknod called\n");
	return 0;
}

static int fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	printf("readdir called, path: %s\n", path);
	return sys_readdir(path, buf, filler, offset);
}


static int fs_open(const char *path, struct fuse_file_info *fi) {
	printf("open called\n");
	return 0;
}

static int fs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	printf("read called\n");
	return 0;
}

static int fs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	printf("write called\n");
	return 0;
}

static struct fuse_operations fs_oper = {
	.getattr	= fs_getattr,
	.mkdir		= fs_mkdir,
	.mknod		= fs_mknod,
	.readdir	= fs_readdir,
	.open		= fs_open,
	.read		= fs_read,
	.write		= fs_write,
};

int main(int argc, char *argv[]) {
	// TODO: mkfs();
	printf("File system mounted!\n");
	return fuse_main(argc, argv, &fs_oper, NULL);	
}
