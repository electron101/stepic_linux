#include<stdio.h>

int client()
{

	int s = socket(AF_INET, SOCK_STREAM, 0);

	inet_aton("127.0.0.1", &local.sin_addr);
	local.sin_port = htons(1234);
	local.sin_family = AF_INET;

	connect(s, (struct socaddr*) &local, sizeof(local));

	/* int result = bind(s, (struct sockaddr_in*) &local, sizeof(local)); */
	/* printf("%d\n", result); */

	char buf[BUFSIZ] = "Hello\n";
	write(s, buf, strlen(buf) + 1);
	close(s);
}


int main(int argc, char **argv)
{
	if (argc != 2) 
		return printf("Use: %s [s|c]\n", argv[0]);

	struct sockaddr_in local;

	if(argv[1][0] == 's') {
		server();
	}

	if(argv[1][0] == 'c') {
		client();
	}




}
