COMPILER = gcc
FILESYSTEM_FILES = syscall.h common.h util.h util.c medfs.c syscall.c 

build: $(FILESYSTEM_FILES)
	$(COMPILER) $(FILESYSTEM_FILES) -o medfs `pkg-config fuse --cflags --libs`
clean:
	rm medfs
