/* Программа читает 100 байт из файла 
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int c, const char **v)
{
	char   buf[100];
	size_t sz = 0;

	if (c != 2) {
		printf("\nПрограмма\n\t%s имя файла\n", v[0]);
		return 1;
	}

	int fd = open(v[1], O_RDONLY | __O_PATH);
	/* int fd = open(v[1], O_RDONLY | O_NOFOLLOW); */
	perror("fd");
	printf("fd = %d\n", fd);
	sz = read(fd, buf, 100);

	printf("sz = %ld\n", sz);

	return 0;
}
