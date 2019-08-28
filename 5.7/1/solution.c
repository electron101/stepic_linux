#include<stdio.h>
#include<netdb.h>
#include<arpa/inet.h>

int main(int argc, char **argv)
{
	if(argc != 2)
		return -1;

	struct hostent *h;
	h = gethostbyname(argv[1]);

	if(h == NULL) {
		printf("error\n");
		return -1;
	}
	
	int i = 0;
	while(h->h_addr_list[i] != NULL) {
		struct in_addr *a = (struct in_addr*)h->h_addr_list[i];
		printf("%s\n", inet_ntoa(*a));
		i++;
	}
	return 0;
}
