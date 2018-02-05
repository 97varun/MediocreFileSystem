#include<stdio.h>
#include <string.h>
#include <libgen.h>
typedef struct a{
	int a;
}a;
char * get_base_path(const char *);
a k={.a=1};
int main(){
	//printf("A");
	printf("%s",get_base_path("a"));
}
char * get_base_path(const char * path){
	char *temp=strdup(path);

	//printf("%s",path);
	return basename(path);
	
}