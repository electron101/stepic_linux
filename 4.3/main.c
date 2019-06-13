#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void mysignal_handler(int signalno)
{
	printf("Called with %d\n", signalno);
}

int main()
{
	int c = 0;

	signal(SIGINT, mysignal_handler);

	while (1) {
		printf("Hello\n", c++);
		usleep("500000");
	}
	return 0;
}

