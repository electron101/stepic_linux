/* map-noblock.c */

#include <fcntl.h>
#include <stdio.h>
#include <sys/poll.h>
#include <unistd.h>

int main(void) 
{
	struct pollfd fds[2];
	char   buf[4096];
	int    i;
	int    rc;

	/* открыть оба канала */
	if ( (fds[0].fd = open("p1", O_RDONLY | O_NONBLOCK)) < 0 ) {
		perror("open p1");
		return 1;
	}

	if ( (fds[1].fd = open("p2", O_RDONLY | O_NONBLOCK)) < 0 ) {
		perror("open p2");
		return 1;
	}

	/* начать чтение из обоих файловых дескрипторов */
	fds[0].events = POLLIN;
	fds[1].events = POLLIN;

	/* пока наблюдаем за одним из fds[0] или fds[1] */
	while ( fds[0].events || fds[1].events ) {
		if ( poll(fds, 2, 0) < 0 ) {
			perror("poll");
			return 1;
		}

		/* проверить, какой из файлоых дескрипторов 
		 * готов для чтения из него */
		for (i = 0; i < 2; i++) {
			if (fds[i].revents) {
				/* fds[i] готов для чтения, 
				 * двигаться дальше ... */
				rc = read(fds[i].fd, buf, sizeof(buf) - 1 );
				if (rc < 0) {
					perror("read");
					return 1;
				} 
				else if (!rc) {
					/* этот канал эакрыт, не пытаться 
					 * читать из него снова */
					fds[i].events = 0;
				}
			       	else { 
					buf[rc] = '\0';
				       	printf("чтение: %s", buf);
				}
			}
		}
	}
	return 0;
}
