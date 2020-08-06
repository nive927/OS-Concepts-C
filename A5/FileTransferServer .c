#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio_ext.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	printf("\nSERVER PROGRAM\n");

	int id;
	char *a, line[50], str[100];
	FILE *fptr;

	id = shmget(111, 50, 00666);
	a = shmat(id, 0, 0);

	sleep(5);

	printf("\nNAME OF THE FILE: %s\n", a);
	fptr = fopen (a, "r");

	while (fgets(line, sizeof(line), fptr))
		strcat(str, line);

	fclose (fptr);

	strcpy(a, str);
	//printf("\nCONTENTS OF THE FILE:\n%s", a);

	shmdt(a);
	shmctl(id, IPC_RMID, NULL);



}
