/* В задании требуется доработать демон, разработанный ранее в задании 6  модуля
 * 3.5.  Задача --  снабдить  демон  обработчиком  сигнала  SIGURG,  по  приходу
 * которого демон должен завершать свою работу. 
 *
 * Пример вызова 
 * ./solution 13336
 *
 * Вывод 
 * Программа выводит в стандартный поток PID созданного
 * демона. (Строка завершается символом конца строки) 
 */

#include <stdio.h>    //printf(3)
#include <stdlib.h>   //exit(3)
#include <string.h>   //memset
#include <unistd.h>   //fork(3), chdir(3), sysconf(3)
#include <signal.h>   //signal(3)
#include <sys/stat.h> //umask(3)
#include <syslog.h>   //syslog(3), openlog(3), closelog(3)

void catcher(int signum)
{
	/* if (signum == SIGURG || signum == SIGTERM) { */
	if (signum == SIGURG) {
		syslog (LOG_NOTICE, "First daemon terminated.");
		closelog();
		exit(-1);
	}
}

static void skeleton_daemon()
{
	struct sigaction act;
	
	pid_t pid;

	/* Отключим родитеский процесс */
	pid = fork();

	/* Произошла ошибка */
	if (pid < 0)
		exit(EXIT_FAILURE);

	/* Успех: пусть родитель прекратит работу */
	if (pid > 0)
		exit(EXIT_SUCCESS);

	/* В случае успеха: дочерний процесс становится лидером сессии */
	if (setsid() < 0)
		exit(EXIT_FAILURE);

	 /* Ловить, игнорировать и обрабатывать сигналы */
	 // TODO: реализовать работающий обработчик сигнала */
	signal(SIGCHLD, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	/* добавляем обрабоку SIGURG  */
	memset(&act, 0, sizeof(act));
	act.sa_handler = &catcher;
	sigaction(SIGURG, &act, NULL);
	/* sigaction(SIGTERM, &act, NULL); */


	/* ----------------------------------------------- */
	/* Этот блок не использую, так как процесс         */
	/* перестаёт быть лидером сеанса (pid != sid)      */
	/* ----------------------------------------------- */
        /*  */
	/* #<{(| Fork во второй раз |)}># */
	/* pid = fork(); */
        /*  */
	/* #<{(| Произошла ошибка |)}># */
	/* if (pid < 0) */
	/* 	exit(EXIT_FAILURE); */
        /*  */
	/* #<{(| Успех: пусть родитель прекратит работу |)}># */
	/* if (pid > 0) */
	/* 	exit(EXIT_SUCCESS); */
        /*  */
	/* ----------------------------------------------- */

	/* Установить новые права доступа к файлу */
	umask(0);

	/* Изменить рабочий каталог на корневой каталог */
	/* или другой соответствующий каталог */
	if (chdir("/") == -1)
		exit(EXIT_FAILURE);

	printf("%d\n", getpid());

	/* Закрыть все дескрипторы открытых файлов
	 * _SC_OPEN_MAX максимальное количество файлов, 
	 * которое может открыть процесс; 
	 * соответствующий макрос - OPEN_MAX. 
	 */
	int x;
	for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
		close (x);
	}

	/* Открыть лог файл */
	openlog ("Nostromo", LOG_PID, LOG_DAEMON);
}


int main()
{
	skeleton_daemon();

	/* =notice	/var/log/messages */
	syslog (LOG_NOTICE, "First daemon started.");
	
	for (;;) {
		//TODO: Вставить код демона сюда.

		/* =notice	/var/log/messages */
		/* syslog (LOG_NOTICE, "First daemon started."); */

		/* sleep (20); */
		/* break; */
	}

	syslog (LOG_NOTICE, "First daemon terminated.");
	closelog();

	return EXIT_SUCCESS;
}
