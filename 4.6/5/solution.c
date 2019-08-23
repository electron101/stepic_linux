/*  В системе существуют 2 региона разделяемой памяти, заполненной некоторыми
 *  числами (типа int). Каждый из регионов имеет размер 1000 байт. Вам
 *  требуется разработать приложение, которое попарно суммирует первые 100
 *  чисел в этих регионах и помещает суммы в новый (созданный вашим
 *  приложением) регион памяти размером 1000 байт. Таким образом, после
 *  завершения работы Вашего приложения в памяти должен существовать регион
 *  разделяемой памяти размером 1000 байт, содержащий в начале 100 сумм. Перед
 *  завершением работы приложение выводит в стандартный поток ввода-вывода ключ
 *  созданного региона, завершающийся символом конца строки. На вход ваше
 *  приложение принимает ключи существующих регионов памяти.  
 *  Пример вызова
 *  ./solution 456764 456768 
 *  512997 
 *  Вывод
 *  Программа выводит в стандартный поток вывода ключ созданного региона памяти
 *  (Строка завершается символом конца строки)
 */

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>


int main(int argc, char **argv)
{
	int   shmid_1;
	int   shmid_2;
	int   shmid_3;
	key_t key_1;
	key_t key_2;
	key_t key_3;
	char  *shm_1;
	char  *shm_2;
	char  *shm_3;
	/* char  *s_1; */
	/* char  *s_2; */
	/* char  *s_3; */
	int  *s_1;
	int  *s_2;
	int  *s_3;
	int   i;



	key_1 = atoi(argv[1]);
	key_2 = atoi(argv[2]);

	/* key_1 = 23423423; */
	/* key_2 = 34564645; */
	
	key_3 = ftok("/dev/null", 's');
	/* key_3 = ftok(".solution", 's'); */
	/* key_3 = 234234234; */

	if ( (shmid_1 = shmget(key_1, 1000, IPC_CREAT | 0666)) < 0 ) {
		perror("shmget1");
		exit(1);
	}
	
	if ( (shmid_2 = shmget(key_2, 1000, IPC_CREAT | 0666)) < 0 ) {
		perror("shmget2");
		exit(1);
	}
	
	if ( (shmid_3 = shmget(key_3, 1000, IPC_CREAT | 0666)) < 0 ) {
		perror("shmget3");
		exit(1);
	}

	if ( (shm_1 = shmat(shmid_1, NULL, 0)) == (char *) -1 ) {
		perror("shmat");
		exit(1);
	}
	
	if ( (shm_2 = shmat(shmid_2, NULL, 0)) == (char *) -1 ) {
		perror("shmat");
		exit(1);
	}
	
	if ( (shm_3 = shmat(shmid_3, NULL, 0)) == (char *) -1 ) {
		perror("shmat");
		exit(1);
	}


	s_1 = (int *)shm_1;
	s_2 = (int *)shm_2;
	s_3 = (int *)shm_3;

	for (i = 0; i < 100; ++i) {
		*s_3 = (int)*s_1 + (int)*s_2;
		/* printf("%d, %d, %d, %d\n", *s_1, *s_2, *s_3, i); */
		*s_3++;
		*s_2++;
		*s_1++;
	}

	printf("%d\n", key_3);
	
	return 0;
}
