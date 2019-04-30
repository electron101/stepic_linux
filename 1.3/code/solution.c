#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <ctype.h>

int (*some)(int);

int init_library(char **argv) 
{
	void *handle = dlopen(argv[1], RTLD_LAZY);
	if( handle == NULL )
		return -1;

	some = (int (*)(int))dlsym(handle, argv[2]);
	if( some == NULL )
		return -2;

	return 1;
}

int main(int argc, char **argv)
{
	if ( argc != 4)
		return 1;

	int N = 0;

	switch ( init_library(argv) ) {
		case 1:
			N = some(atoi(argv[3]));
			fprintf(stdout, "%d\n", N);
			break;
		case -1:
			printf("libraty not loaded\n");
			return 1;
		case -2:
			printf("funk not loaded\n");
			return 1;
	}
	
	return 0;
}

/*
 * Коментария одного из участников курса
 * способ номер #1, который рассматривается тут.

//Объявляем указатель на функцию
int (*func_ptr)(int);
// dlopen ...
//получаем адрес функции
func_ptr = (int(*)(int)) dlsym(lh, "some_func");
// вызываем функцию
(*func_ptr)(42); либо func_ptr(42) разницы нет.
Но такой способ мне не нравится т.к. при сборке с pedantic будут ворнинги типа:
warning: ISO C forbids conversion of object pointer to function pointer type [-Wpedantic]

способ #2, который использую я, в нем нет явного преобразования.
// объявляем тип, который является указателем на функцию с необходимой сигнатурой:
typedef int(*func_ptr)(int);
// задаем объединение
union {
          void *p;
          func_ptr f;
    } funcp;
//получаем адрес функции
funcp.p = dlsym(lh, "some_func");
// вызываем функцию
funcp.f(42);

gcc -Wall -Wextra -pedantic dl.c -o dl -ldl не выдаст ни одного предупреждения.
*/
