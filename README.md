# MediocreFileSystem  Phase-1
## How to use
  * make
  * Mount using ./medfs [mountpoint] .Use -f to print out warning/wtever messages you want to be printed.
  * Unmount using fusermount -u [mountpoint]
## Description
  * After mounting if "ls" or any other syscall that reads the directory is called, then the buffer is filled with 2 files called "kys" and "fys"
  * Hardcoded contents of these files do not occupy memory
