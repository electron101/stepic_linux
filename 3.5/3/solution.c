/* Разработать программу solution, которая по заданному pid, осуществляет  поиск
 * пути в дереве процессов до процесса с идентификатором 1 (init).  Для  каждого
 * найденного	процесса  печатается  в  отдельной  строке  его   идентификатор.
 *
 * Пример вызова 
 * ./solution 1766 1766 1299 465 1
 *
 * Представление решения 
 * Решение предоставляется в виде файла solution.c,
 * содержащего исходный код Вашего приложения.  
 *
 * Вывод 
 * Программа  выводит	в  стандартный	поток  вывода  числа  (каждое  число   в
 * отдельной строке)
 */

#define _POSIX_SOURCE
/* #define _GNU_SOURCE */
#include <stdio.h>
#include <stdlib.h>
#include <linux/fcntl.h>

void trace_pid2init(int pid)
{
	printf ("%d\n", pid);
	
	if (pid == 1)
		exit(EXIT_SUCCESS);

	/* искать Ppid в stat */
	int  fd;
	char ch;
	char ppid[10];
	int  count = 0;
	int  i = 0;
	char file_name[300];
	
	snprintf(file_name, sizeof(file_name) - 1,
			"/proc/%d/stat", pid);

	fd = open(file_name, O_RDONLY);

	while (read (fd, &ch, 1) > 0) {
		if (ch == ' ') {
			if (i != 0) {
				ppid[i] = '\0';
				break;
			}
			else
				count++;
			continue;
		}

		if (count == 3) {
			ppid[i] = ch;
			i++;
		}
	}

	trace_pid2init(atoi(ppid));
}

int main(int argc, char **argv)
{
	if (argc != 2)
		exit(EXIT_FAILURE);

	trace_pid2init(atoi(argv[1]));

	exit(EXIT_SUCCESS);
}

