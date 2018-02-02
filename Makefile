COMPILER = gcc
FILESYSTEM_FILES = medfs.c

build: $(FILESYSTEM_FILES)
	$(COMPILER) $(FILESYSTEM_FILES) -o medfs `pkg-config fuse --cflags --libs`
clean:
	rm medfs
