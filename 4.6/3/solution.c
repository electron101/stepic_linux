/* Разработать приложение, умеющее обрабатывать сигналы SIGUSR1 ,SIGUSR2,
 * SIGTERM. После старта Ваше приложение должно по приходу одного из сигналов
 * SIGUSR1,  SIGUSR2 выполнять суммирование числа срабатываний каждого из
 * сигналов, а после прихода сигнала SIGTERM, требуется вывести в стандартный
 * поток вывода 2 числа, разделенных пробелом, соответствующих количеству
 * обработанных сигналов SIGUSR1, SIGUSR2, и завершить программу. Вывод
 * оканчивается символом конца строки. 
 *
 * Пример вызова 
 * ./solution 79 38
 * Вывод 
 * Программа выводит в стандартный поток вывода 2 числа,
 * разделенных пробелом (Строка завершается символом конца строки) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

void catcher(int);

int count_sigusr1 = 0;
int count_sigusr2 = 0;


int main()
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = &catcher;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	sigaction(SIGTERM, &act, NULL);

	for (;;){
	}

	return 0;
}

void catcher(int signum)
{
	if (signum == SIGUSR1) {
		count_sigusr1++;
		return;
	} 

	if (signum == SIGUSR2) {
		count_sigusr2++;
		return;
	} 
	
	if (signum == SIGTERM) {
		fprintf(stdout, "%d %d\n", count_sigusr1, count_sigusr2);
		fflush(stdout);
		exit(-1);
	}
}
