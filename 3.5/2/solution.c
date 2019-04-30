/* Разработать программу solution, которая осуществляет поиск  и  подсчет  числа
 * процессов с именем genenv в системе Linux на которой  выполняется  программа.
 * 
 * Пример вызова 
 * ./solution 13 
 *
 * Представление решения 
 * Решение предоставляется в виде файла  solution.c,  содержащего  исходный  код
 * Вашего приложения.
 *
 * Вывод
 * Программа выводит в стандартный поток вывода число в отдельную строку(  вывод
 * должен оканчиваться на символ перевода строки \n).
 */

#define _DEFAULT_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	struct dirent **namelist;
	int    n;
	char   buf_name[300];
	char   line[1024];
	char   *result;
	FILE   *fp;
	int    count = 0;

	n = scandir("/proc", &namelist, NULL, alphasort);
	
	if (n == -1) {
		perror("scandir");
		exit(EXIT_FAILURE);
	}

	while (n--) {
		snprintf (buf_name, sizeof(buf_name) - 1, 
				"/proc/%s/stat", namelist[n]->d_name);

		if ( (fp = fopen(buf_name, "r")) == NULL ) {
			continue;
		}

		fgets(line, 1024, fp);

		result = strstr(line, "genenv");
		if (result != NULL)
			count++;

		free(namelist[n]);
	}

	free(namelist);
	
	printf("%d\n", count);

	exit(EXIT_SUCCESS);
}
