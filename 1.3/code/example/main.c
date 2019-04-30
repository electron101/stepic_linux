#include <stddef.h>
#include <stdio.h>
#include <dlfcn.h>
void (*hello_message)(const char *);


int init_library() 
{
	void *handle = dlopen("./libHello.so", RTLD_LAZY);
	if( handle == NULL )
		return -1;

	hello_message = (void (*)(const char *))dlsym(handle, "hello_message");
	if( hello_message == NULL )
		return -2;

	return 1;
}


int main()
{
	if( init_library() == 1 )
		hello_message("Петя!");
	
	if( init_library() == -1 )
		printf("Бибилотека не загрузилась\n");
	
	if( init_library() == -2 )
		printf("Функция hello_message не вызвана\n");

	return 0;
}
