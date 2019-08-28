#include	<arpa/inet.h>		/* inet(3) functions */
#include	<errno.h>
#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<sys/un.h>		/* for Unix domain sockets */
#include	<time.h>

#define		MAXLINE	4096		/* max text line length */
#define		LISTENQ	1024		/* 2nd argument to listen() */
#define		SA	struct sockaddr


void ascii_sort(char *s)
{
	int  i, j;
	char tmp;

	for (i = 0; i < strlen(s); ++i) {
		for (j = 1; j < strlen(s); ++j) {
			if (s[i] == '\0') 
				return;
			if ( (int)s[i] > (int)s[j] ) {
				tmp = s[i];
				s[i] = s[j];
				s[j] = tmp;
			}
		}
	}
}

int compare(const void * x1, const void * x2)
{
	return ( *(char*)x2 - *(char*)x1 );
}

int Socket(int family, int type, int protocol)
{
	int	n;

	if ( (n = socket(family, type, protocol)) < 0)
		perror("socket error");
	return(n);
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (bind(fd, sa, salen) < 0)
		perror("bind error");
}

void Listen(int fd, int backlog)
{
	char	*ptr;

	/*4can override 2nd argument with environment variable */
	if ( (ptr = getenv("LISTENQ")) != NULL)
		backlog = atoi(ptr);

	if (listen(fd, backlog) < 0)
		perror("listen error");
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int	n;

again:
	if ( (n = accept(fd, sa, salenptr)) < 0) {
#ifdef	EPROTO
		if (errno == EPROTO || errno == ECONNABORTED)
#else
		if (errno == ECONNABORTED)
#endif
			goto again;
		else
			perror("accept error");
	}
	return(n);
}

void Write(int fd, void *ptr, size_t nbytes)
{
	if (write(fd, ptr, nbytes) != nbytes)
		perror("write error");
}

int Read(int fd, void *ptr, size_t nbytes)
{
	if (read(fd, ptr, nbytes) == -1)
		perror("read error");
	return nbytes;
}

void Close(int fd)
{
	if (close(fd) == -1)
		perror("close error");
}

int main(int argc, char **argv)
{
	int    listenfd,   connfd;
	struct sockaddr_in servaddr;
	char   buff[MAXLINE];
	int    nread;
	int    i;

	if (argc != 2)
		return -1;
	
	for (i = 0; i < argc; ++i) 
		if ( isdigit(argv[1][i]) == 0 )
			return -1;

	/* создание сокета, такое же как и в коде 
	 * клиента, используется функция обёртка
	 * Socket - определена выше 
	 */
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;

	/* INADDR_ANY - позволяет серверу принимать 
	 * соединение от клиента на любом интерфейсе
	 * htonl (host to network long) - преобразует ip адрес
	 * в требуемый формат - в сетевой порядок байт 
	 */
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(atoi(argv[1]));	

	/* порт сервера связывается с сокетом путём заполнения 
	 * структуры адреса интернет-сокета 
	 */
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	/* преобразование сокета в прослушиваемый сокет.
	 * три этапа которые обычны для любого сервера TCP:
	 * socket, bing, listen. 
	 * LISTENQ - максимальное количество клиентских соеединений 
	 */
	Listen(listenfd, LISTENQ);
	connfd = Accept(listenfd, (SA *) NULL, NULL);

	/* Приём клиетского соединения, отправка ответа 
	 */
	for ( ; ; ) {		/* бесконечный цикл */
		/* используется трёхэтапное рукопожатие 
		 */

		bzero(&buff, sizeof(buff));
		/* читаем с клиента */
		nread = Read(connfd, buff, MAXLINE);
		if ( strncmp(buff, "OFF", 3) == 0 ) {
			Close(connfd);
			return -1;
		}

		/* ответ клиенту */
		ascii_sort(buff);
		qsort(buff, strlen(buff), 1, compare);
		Write(connfd, buff, strlen(buff));

		/* Close(connfd); */
	}
}
