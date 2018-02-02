
 
#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

static int do_getattr( const char *path, struct stat *st )
{
	//printf( "[getattr] Called\n" );
	
	st->st_uid = getuid(); 
	st->st_gid = getgid(); 
	st->st_atime = time( NULL ); 
	st->st_mtime = time( NULL ); 
	
	if ( strcmp( path, "/" ) == 0 )
	{
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 2; 
	}
	else
	{
		st->st_mode = S_IFREG | 0644;
		st->st_nlink = 1;
		st->st_size = 1024;
	}
	
	return 0;
}

static int do_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi )
{
	printf( "Readdir called", path );
	
	filler( buffer, ".", NULL, 0 ); // Current Directory
	filler( buffer, "..", NULL, 0 ); // Parent Directory
	
	if ( strcmp( path, "/" ) == 0 ) // If the user is trying to show the files/directories of the root directory show the following
	{
		filler( buffer, "kys", NULL, 0 );
		filler( buffer, "fys", NULL, 0 );
	}
	
	return 0;
}
/*static int do_open(const char *, struct fuse_file_info *){


};
*/
static int do_read( const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi )
{
	printf( "Reading files %s, %ld, %ld\n", path, offset, size );
	//const char msg[] = "Hello World!";
	//write(STDOUT_FILENO, "msg", sizeof(msg)-1);
	char file54Text[] = "This file says please kys ";
	char file349Text[] = "This file says go fys";
	char *selectedText = NULL;
	
	
	
	if ( strcmp( path, "/kys" ) == 0 )
		selectedText = file54Text;
	else if ( strcmp( path, "/fys" ) == 0 )
		selectedText = file349Text;
	else
		return -1;

	memcpy( buffer, selectedText + offset, size );
		
	return strlen( selectedText ) - offset;
}

static struct fuse_operations operations = {
    .getattr	= do_getattr,
    .readdir	= do_readdir,
    .read		= do_read,
};

int main( int argc, char *argv[] )
{
	return fuse_main( argc, argv, &operations, NULL );
}
