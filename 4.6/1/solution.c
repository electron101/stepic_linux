/*
 * Некоторая утилита генерирует  довольно  большой  вывод,  а  вам  требуется  *
 * всего-лишь посчитать количество символов '0'  в  этом  выводе.   Утилита  при
 * запуске принимает 1 параметр.  Вам требуется разработать  программу,  которая
 * вызывает указанную утилиту, с заданным параметром и	подсчитывает  количество
 * символов '0' в ее выводе.  Ваша программа принимает на вход	2  параметра  --
 * имя утилиты, в текущем каталоге и ее параметр.  Ваша программа  должна  после
 * подсчета вывести найденное число  '0'  в  отдельной	строке,  заканчивающейся
 * символом конца  строки.   
 *
 * Пример  вызова  
 * ./solution  someprog  param  234222
 *
 * Представление решения
 *
 * Решение предоставляется в виде двух  файлов  solution.c
 * и Makefile, в последнем предполагается цель по умолчанию, которая приводит  к
 * сборке Вашего приложения.  Бинарный файл  вашего  решения  должен  иметь  имя
 * solution. 
 *
 * Вывод
 *
 * Программа выводит в стандартный поток вывода число  (в  *  отдельной  строке)
 */

#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_LEN 100
#define DATA_SIZE 4096

int  count_zero_in_str(char *s);

int main(int argc, char *argv[])
{
	FILE *pf;
	char command[COMMAND_LEN];
	char data[DATA_SIZE];
	char cmd[20];
	int  sum_zero = 0;
	
	memset(cmd, '\0', COMMAND_LEN);
	snprintf(cmd, COMMAND_LEN - 1, "%s %s", argv[1], argv[2]);

	// Execute a process listing
	sprintf(command, cmd); 

	// Setup our pipe for reading and execute our command.
	pf = popen(command,"r");

	if(!pf) {
		fprintf(stderr, "Could not open pipe for output.\n");
		exit(1);
	}

	// Grab data from process execution
	while( fgets(data, DATA_SIZE , pf) ) {
		// Print grabbed data to the screen.
		/* fprintf(stdout, "%s",data); */

		/* суммируем количество символов '0' */
		sum_zero += count_zero_in_str(data); 
	}
	
	printf("%d\n", sum_zero);

	if (pclose(pf) != 0)
		fprintf(stderr," Error: Failed to close command stream \n");

	return 0;
}

/* Возвращает количество нулей (символов 0) в строке.
 * Читаем строку посимвольно, проверяя каждый символ на равенство 0
 */
int count_zero_in_str(char *s)
{
	int i;
	int count_zero = 0;

	for(i = 0; i < strlen(s); ++i) 
		if (s[i] == '0')
			count_zero++;
	return count_zero;
}
