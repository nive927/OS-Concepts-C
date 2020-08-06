#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	printf("\nCLIENT PROGRAM\n");

	int pid, id;
	char *a, *b;

	id = shmget(111, 50, IPC_CREAT | 00666);
	pid = fork();

	if(pid > 0)
	{
		a = shmat(id, 0, 0);

		printf("Enter the name of the file: ");
		scanf("%[^\n]%*c", a);

		wait(0);
		shmdt(a);
	}

	else
	{
		sleep(15);

		b = shmat(id, 0, 0);

		while(b[0] == '\0');

		printf("\nRECEIVED FILE CONTENTS:\n%s\n", b);
		shmdt(b);
	}

	shmctl(id, IPC_RMID, NULL);
	return 0;
}