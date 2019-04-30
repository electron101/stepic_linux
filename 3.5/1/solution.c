/* Разработать программу solution, которая осуществляет поиск родительского  PID
 * текущего процесса в файловой системе proc и	выводит  найденное  значение  на
 * консоль.  Результат,  возвращаемый  функций	должен	быть  выведен  отдельной
 *  строкой(должен  оканчиваться  символом  перевода   строки	\n)   в   stdio.
 * 
 * Пример вызова
 * ./solution 13221
 *
 * Представление решения
 * Решение предоставляется в виде файла  solution.c,  содержащего  исходный  код
 * Вашего приложения.
 *
 * Вывод
 * Программа выводит в стандартный поток вывода число в  отдельную  строку(вывод
 * должен оканчиваться символом перевода строки \n).
 */

#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <linux/fcntl.h> /* open(), O_XXX flags */
#include <unistd.h>	 /* close(), getpid() */

int main()
{
	int  fd;
	char ch;
	char ch_pid[10];
	int  count = 0;
	int  i = 0;

	fd = open("/proc/self/stat", O_RDONLY);

	while (read (fd, &ch, 1) > 0) {
		if (ch == ' ') {
			if (i != 0) {
				ch_pid[i] = '\0';
				break;
			}
			else
				count++;
			continue;
		}

		if (count == 3) {
			ch_pid[i] = ch;
			i++;
		}
	}

	printf ("%s\n", ch_pid);
	return 0;
}
