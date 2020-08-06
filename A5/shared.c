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
	int pid, id;
	char *a, *b;

	id = shmget(111, 50, IPC_CREAT | 00666);
	pid = fork();

	if(pid > 0)
	{
		printf("\nPARENT PROCESS\n");

		a = shmat(id, 0, 0);

		printf("Enter a string: ");
		scanf("%[^\n]%*c", a);

		wait(0);
		shmdt(a);
	}

	else
	{
		b = shmat(id, 0, 0);

		while(b[0] == '\0');

		for(int i=0; b[i] != '\0'; i++)
			b[i] -= (b[i] >= 'a' && b[i] <='z') ? 32 : 0;

		printf("\nCHILD PROCESS\nUppercase: %s\n", b);
		shmdt(b);
	}

	shmctl(id, IPC_RMID, NULL);
	return 0;

}