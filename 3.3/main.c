#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <unistd.h>

#define STACK_SIZE 10000
char child_stack[STACK_SIZE + 1];


int child(void *params)
{
	int c = 0;

	while(1) {
		usleep(500);
		printf("child turn %d\n", c++);
	}
}


int main()
{
	int c = 0;
	int result = clone(child, child_stack + STACK_SIZE, CLONE_PARENT, 0);
	printf("clone result = %d\n", result);

	while(1) {
		usleep(500);
		printf("parent turn %d\n", c++);
	}
}
