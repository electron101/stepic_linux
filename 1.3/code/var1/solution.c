#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

int (*someSecretFunctionPrototype)(int);

int init_library(char **argv) 
{
	/* void *handle = dlopen("./libShared.so", RTLD_LAZY); */
	void *handle = dlopen(argv[1], RTLD_LAZY);
	if( handle == NULL )
		return -1;

	someSecretFunctionPrototype = (int (*)(int))dlsym(handle, argv[2]);
	if( someSecretFunctionPrototype == NULL )
		return -2;

	return 1;
}


int main(int argc, char **argv)
{
	if ( argc != 4)
		return 0;

	switch ( init_library(argv) ) {
		case 1:
			printf("%d\n", someSecretFunctionPrototype(atoi(argv[3])));
			/* printf("%d\n", someSecretFunctionPrototype(8)); */
			break;
		case -1:
			printf("Бибилотека не загрузилась\n");
			break;
		case -2:
			printf("Функция не вызвана\n");
			break;
	}
	
	return 0;
}
