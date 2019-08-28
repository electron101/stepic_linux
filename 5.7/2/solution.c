#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<arpa/inet.h>

struct sockaddr_in local;

int main(int argc, char **argv)
{
	int  i;
	int  s;
	int  result;
	char buf[BUFSIZ];
	int  nread;

	if (argc != 2)
		return -1;
	
	for (i = 0; i < argc; ++i) 
		if ( isdigit(argv[1][i]) == 0 )
			return -1;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	inet_aton("127.0.0.1", &local.sin_addr);
	local.sin_port = htons(atoi(argv[1]));
	local.sin_family = AF_INET;

	result = bind(s, (struct sockaddr_in*) &local, sizeof(local));

	for (;;) {
		bzero(&buf, sizeof(buf));
		nread = read(s, buf, BUFSIZ);
		if ( strcmp(buf, "OFF\n") == 0 )
			return -1;
		
		if (nread <= 5000)
			printf("%s\n", buf);
	}
}
