/*
 * Разработать программу solution, которая при запуске себя "демонизирует" и
 * остается в  памяти. Перед закрытием стандартного потока вывода stdout
 * унаследованного от родителя, программа должна вывести в него Pid
 * процесса-демона.
 * Пример программы которая демонизирует процесс, пишет несколько логов,
 * и выводит свой pid на печать. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

static void skeleton_daemon()
{
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
	/* SIG_IGN — для игнорирования сигнала 
	 * SIGHUP Закрытие терминала 
	 * SIGCHLD Дочерний процесс завершен или остановлен  */
	signal(SIGCHLD, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	/* Установить новые права доступа к файлу */
	umask(0);

	/* Изменить рабочий каталог на корневой каталог */
	/* или другой соответствующий каталог */
	chdir("/");

	/* Закрыть все дескрипторы открытых файлов
	 * _SC_OPEN_MAX максимальное количество файлов, 
	 * которое может открыть процесс; 
	 * соответствующий макрос - OPEN_MAX. 
	 */

	printf("%d\n", getpid());

	int x;
	for (x = sysconf(_SC_OPEN_MAX); x>=0; x--) {
		close (x);
	}

	/* Открыть лог файл */
	openlog ("Nostromo", LOG_PID, LOG_DAEMON);
}


int main()
{
	skeleton_daemon();

	for (;;) {
		//TODO: Вставить код демона сюда.

		/* =notice	/var/log/messages */
		syslog (LOG_NOTICE, "First daemon started.");
		/* sleep (20); */
		/* break; */
	}

	syslog (LOG_NOTICE, "First daemon terminated.");
	closelog();

	return EXIT_SUCCESS;
}
