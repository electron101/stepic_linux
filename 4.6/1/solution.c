#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 100

int count_zero_in_str(char *s);
char *readline(int fd, char * buffer); 

int main(int argc, char *argv[])
{
	pid_t cpid;
	int   fd[2];
	char  str[MAX_BUF];
	char  cmd[20];
	int   sum_zero = 0;
	char  *v[3];

	/* обнулим переменную cmd */
	memset(cmd, '\0', 20);
	snprintf(cmd, 19, "%s %s", argv[1], argv[2]);
	v[0] = argv[1];
	v[1] = argv[2];
	v[2] = (char *)NULL;
	
	if (pipe(fd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	
	cpid = fork();
	if (cpid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (cpid == 0) {		/* Потомок записывает в канал */
		/* Записываем в канал */
		
		close(1);	/* закроем стандартный вывод (stdout) */
		dup2(fd[1], 1);	/* отождествляем стандарный вывод (stdout) 
				  с файловым дискриптором канала, 
				  предназначенным для записи */

		close(fd[1]);	/* закрыли файловый дескриптор канала, 
				предназначенный для записи */
		
		close(fd[0]);	/* закрыли файловый дескриптор канала, 
				предназначенный для чтения */
		
		/* выполним программу с параметрами */
		/* execlp("/bin/sh","/bin/sh", "-c", "ps -ax", (char *)NULL); */
		/* execvp(argv[1], argv); */
		execvp(v[0], v);
		/* execlp(argv[1], argv[2], (char *)NULL); */
		exit(0);
	} 
	else {			/* Родитель читает из канала */	
	
		close(fd[1]);	/* закрыли файловый дескриптор канала, 
				предназначенный для записи */
		/* бесконечный цикл */	
		/* do {			 */
			memset(str, '\0', MAX_BUF); /* заполним строку 0-ми */

			/* читаем из дискриптора чтения строку до символа \n */
			readline(fd[0], str); 
			
			printf("%s", str);
			
		/* суммируем количество символов '0' */
			sum_zero += count_zero_in_str(str); 
			/* sleep(1); */

		/* } while ( 1 ); */
		printf("sum_zero = %d\n", sum_zero);
		wait(NULL);	/* Ждем окончания потомка */
	}
	return 0;
}

/* Читает из дискриптора в buffer пока не встретит \n 
 * возвращает указатель на строку buffer */
char *readline(int fd, char * buffer) {
	char c;
	int  counter = 0;
	while (read(fd, &c, 1) != 0) {
		buffer[counter++] = c;
		if (c == '\n') {
			buffer[counter] = '\0';
			break;
		}
	}
	return buffer;
}

/* Возвращает количество символов '0' в строке s */
int count_zero_in_str(char *s) 
{
	int i;
	int count_zero = 0;

	for (i = 0; i < strlen(s); i++) {
		if (s[i] = '0')
			count_zero++;	
	}
	return count_zero;
}
