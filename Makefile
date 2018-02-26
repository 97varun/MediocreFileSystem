COMPILER = gcc
HEADER_FILES = common.h syscall.h util.h disk.h
FILESYSTEM_FILES = medfs.c syscall.c util.c disk.c

build: $(FILESYSTEM_FILES) $(HEADER_FILES)
	$(COMPILER) $(FILESYSTEM_FILES) -o medfs `pkg-config fuse --cflags --libs`
clean:
	rm medfs
