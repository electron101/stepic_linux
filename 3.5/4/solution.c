/* Разработать программу solution, которая по заданному pid, осуществляет  поиск
 * всех потомков  процесса  с  этим  идентификатором  и  выводит  их  количество
 * (включая процесс с заданным pid).
 *
 * Пример вызова
 * ./solution 1233 76
 *
 * Представление решения
 * Решение предоставляется в виде файла  solution.c,  содержащего  исходный  код
 * Вашего приложения.
 *
 * Вывод
 * Программа выводит в стандартный поток вывода число в отдельной строке(  число
 * должно оканчиваться символом перевода строки \n)
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/fcntl.h>
#include <dirent.h>
#include <string.h>

void trace_pid2init(char *pid, char *chain)
{
	/* если init то выйти */
	if ( atoi (pid) == 0 ) {
		/* printf ("%s - ", pid); */
		strncat(chain, pid, 127 - strlen(chain));
		strncat(chain, " ", 127 - strlen(chain));
		return;
	}
	
	/* искать Ppid в status */
	FILE   *stream;
	char   *line = NULL;
	size_t len = 0;
	size_t nread;
	char   file_name[256];
	char   ppid[10];
	int    i;
	int    j;
	
	snprintf(file_name, sizeof(file_name) - 1,
			"/proc/%s/status", pid);

	stream = fopen(file_name, "r");
	if (stream == NULL) 
		return;
	
	strncat(chain, pid, 127 - strlen(chain));
	strncat(chain, " ", 127 - strlen(chain));

	/* найдём номер предка ppid */
	while ((nread = getline(&line, &len, stream)) != -1) {
		/* если нашли строку с Ppid */
		if (strstr(line, "PPid:") != NULL) {
			i = 6; /* ppid начинается с 6 символа */
			j = 0;
			while( line[i] ) {
				if ( line[i] == ' ' || line[i] == '\n' || 
						line[i] == '\t' ) {
					ppid[j] = '\0';
					break;
				}

				ppid[j] = line[i];
				i++;
				j++;
			}
			break;
		}
	}
	free(line);

	fclose(stream);
	
	trace_pid2init(ppid, chain);
}

int main(int argc, char **argv)
{
	struct dirent **namelist;
	int    n;
	int    pid_dir;
	char   chain[128];
	int    result = 0;
	char   **list_chain;
	int    i;
	int    t;

	if (argc != 2)
		exit(EXIT_FAILURE);

	n = scandir("/proc", &namelist, NULL, alphasort);
	t = n;
	
	if (n == -1) {
		perror("scandir");
		exit(EXIT_FAILURE);
	}

	list_chain = (char **) malloc(n * sizeof(char*));

	for (i = 0; i < n; ++i) {
		list_chain[i] = (char*)malloc(128 * sizeof(char));
	
		bzero(list_chain[i], sizeof(list_chain[i]));
	}
	
	i = 0;
	while (n--) {
		bzero(chain, sizeof(chain));
		
		if ( pid_dir = atoi(namelist[n]->d_name) ) 
			trace_pid2init(namelist[n]->d_name, chain);
		else 
			continue;

		strcpy(list_chain[i], chain);
		++i;
	
		/* printf("chain = %s\n", chain); */

		free(namelist[n]);
	}

	free(namelist);

	for (i = 0; i < t; ++i) {
		if ( list_chain[i] ) {
			if ( strstr(list_chain[i], argv[1]) != NULL )
				result++;

			/* printf("%s\n", list_chain[i]); */
		}
		free(list_chain[i]);
	}
	free(list_chain);

	printf("%d\n", result);

	exit(EXIT_SUCCESS);
}
