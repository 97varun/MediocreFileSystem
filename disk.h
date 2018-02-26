#ifndef DISK_H
#define DISK_H

#define MAX_BLOCKS 100
#define BLOCK_SZ 512

int init_disk();
int write_block(void*);
int read_block(int, void*);
void free_disk();
int erase_block(int);

#endif
