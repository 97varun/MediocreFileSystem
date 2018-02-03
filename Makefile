COMPILER = gcc
FILESYSTEM_FILES = fs.c syscall.c

build: $(FILESYSTEM_FILES)
	$(COMPILER) $(FILESYSTEM_FILES) -o fs `pkg-config fuse --cflags --libs`
clean:
	rm fs
