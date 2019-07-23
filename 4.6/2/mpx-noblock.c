/* map-noblock.c */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) 
{
	int  fds[2];
	char buf[4096];
	int  i;
	int  fd;

	/* открыть оба канала в неблокирующем режиме */
	if ( (fds[0] = open("p1", O_RDONLY | O_NONBLOCK)) < 0 ) {
		perror("open p1");
		return 1;
	}

	if ( (fds[1] = open("p2", O_RDONLY | O_NONBLOCK)) < 0 ) {
		perror("open p2");
		return 1;
	}

	fd = 0;
	while (1) {
		/* если данные доступны, прочитать и отобразить их  */
		/* EAGAIN - нет доступных данных прямо сейчас, повторите попытку позже */
		i = read (fds[fd], buf, sizeof(buf) - 1);
		if ( (i < 0) && (errno != EAGAIN) ) {
			perror("read");
			return 1;
		}
		else if (i > 0) {
			printf("канал закрыт\n");
			return 0;
		}
		
		buf[i] = '\0';
		printf("чтение: %s", buf);

		/* читать из другого файлового дескриптора  */
		fd = (fd + 1) % 2;
	}
}
