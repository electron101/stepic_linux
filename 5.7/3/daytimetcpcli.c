/* Простой клиент даты и времени
 *
 * Сборка:
 * gcc daytimetcpcli.c -o daytimetcpcli
 */
#include	<arpa/inet.h>		/* inet(3) functions */
#include	<errno.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<sys/un.h>		/* for Unix domain sockets */

#define		MAXLINE	4096		/* max text line length */
#define		SA	struct sockaddr

#include	"../lib/error.c"	/* для функций err_... */

int main (int argc, char **argv)
{
	int    sockfd, n;
	char   recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;

	/* создаём потоковый сокет(SOCK_STREAM) интернета(AF_INET) 
	 */
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("Ошибка сокета");
	
	/* инициализируем всю структуру нулями 
	 */
	bzero(&servaddr, sizeof(servaddr));	
	
	servaddr.sin_family = AF_INET;
	
	/* htons (host to network short) - преобразует двоичный номер
	 * порта в требуемый формат. 13 порт - времени и даты 
	 */
	servaddr.sin_port = htons(atoi(argv[2]));

	/* устанавливаем ip адрес равным первому аргументу ком.строки
	 * inet_pton (presentation to numeric) - преобразует 
	 * из символов ASCII(127.0.0.1) в двоичный формат 
	 */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("Ошибка inet_pton для %s", argv[1]);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("Ошибка соединения");

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0)
	{
		recvline[n] = 0; 
		if (fputs(recvline, stdout) == EOF)
			err_sys("Ошибка fputs");
	}

	if (n < 0)
		err_sys("Ошибка read");

	exit(0);
}
