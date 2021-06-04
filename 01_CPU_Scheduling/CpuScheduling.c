#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define N 20

//FUNCTION TO IMPLEMENT getch() DUE TO THE ABSENCE OF <conio.h> IN gcc COMPILER

char getch()
{
	char buf=0;

	struct termios old={0};

	fflush(stdout);

	if(tcgetattr(0, &old)<0)
		perror("tcsetattr()");

	old.c_lflag&=~ICANON;
	old.c_lflag&=~ECHO;
	old.c_cc[VMIN]=1;
	old.c_cc[VTIME]=0;

	if(tcsetattr(0, TCSANOW, &old)<0)
		perror("tcsetattr ICANON");

	if(read(0,&buf,1)<0)
		perror("read()");

	old.c_lflag|=ICANON;
   	old.c_lflag|=ECHO;

	if(tcsetattr(0, TCSADRAIN, &old)<0)
		perror ("tcsetattr ~ICANON");

	return buf;
}

struct job
{
	int pid;
	int arrival_time;
	float burstTime;
	float turn_around;
	float waiting_time;
	float remaining;
	int start;
	int flag;

};

struct job read_job()
{
	struct job process;

	printf("\n\tProcess ID: P");
	scanf("%d", &process.pid);
	printf("\tArrival Time: ");
	scanf("%d", &process.arrival_time);
	printf("\tBurst Time: ");
	scanf("%f", &process.burstTime);

	process.flag = 0;
	process.remaining = process.burstTime;

	return process;
}

void display_job(struct job process)
{
	printf("\n\n\tProcess ID: P%d", process.pid);
	printf("\n\tArrival Time: %d", process.arrival_time);
	printf("\n\tBurst Time: %.2f", process.burstTime);
}

void fcfs_sort(struct job a[], int n)
{
	int i, j;
	struct job small;

	for(i=1; i<n; i++)
	{
		small = a[i];
		j = i-1;

		while((j >= 0) && (small.arrival_time < a[j].arrival_time))
		{
			a[j+1] = a[j];
			j--;
		}

		a[j+1] = small;
	}
}

void table(struct job ready[], float tot_waiting, float tot_turn, int n)
{
	if(n <= 0)
		return;

	printf("\n\n\tPROCESS DETAILS CHART");

	printf("\n\n\t----------------------------------------------------------------------------------------");
	printf("\n\tProcess ID\tArrival Time\tBurst Time\tWaiting Time\t\tTurnaround Time");
	printf("\n\t----------------------------------------------------------------------------------------");

	for(int i=0; i<n; i++)
	{
		printf("\n\tP%d\t\t%d\t\t%.2f\t\t%.2f\t\t\t%.2f", ready[i].pid, ready[i].arrival_time, ready[i].burstTime, ready[i].waiting_time, ready[i].turn_around);
	}

	printf("\n\t----------------------------------------------------------------------------------------");
	printf("\n\t\t\t\t\tAverage\t\t%.2f\t\t\t%.2f", (tot_waiting/n), (tot_turn/n));
	printf("\n\t----------------------------------------------------------------------------------------");

}

void gantt(struct job arr[], int n, int tot_time)
{
	if(n <= 0)
		return;

	printf("\n\n\tGANTT CHART");

	int i, j;

	// printing the top bar
	printf("\n\n\t ");
	for(i=0; i<n-1; i++)
	{
		for(j=arr[i].start; j<arr[i+1].start; j++)
			printf("--");
		printf(" ");
	}

	for(j=0; j<tot_time - arr[n-1].start; j++)
		printf("--");
	printf(" ");

	printf("\n\t|");

	// printing the process id in the middle
	for(i=0; i<n-1; i++)
	{
		for(j=arr[i].start; j<arr[i+1].start - 1; j++)
			printf(" ");
		printf("P%d", arr[i].pid);

		for(j=arr[i].start; j<arr[i+1].start - 1; j++)
			printf(" ");
		printf("|");
	}

	for(j=0; j<tot_time - arr[n-1].start - 1; j++)
		printf(" ");
	printf("P%d", arr[n-1].pid);
	for(j=0; j<tot_time - arr[n-1].start - 1; j++)
		printf(" ");
	printf("|");

	printf("\n\t ");

	// printing the bottom bar
	for(i=0; i<n-1; i++)
	{
		for(j=arr[i].start; j<arr[i+1].start; j++)
			printf("--");
		printf(" ");
	}

	for(j=0; j<tot_time - arr[n-1].start; j++)
		printf("--");
	printf(" ");

	printf("\n\t");

	// printing the time line
	for(i=0; i<n-1; i++)
	{
		printf("%d", arr[i].start);
		for(j=arr[i].start; j<arr[i+1].start; j++)
			printf("  ");
		if(arr[i].start > 9)
			printf("\b"); // backspace : remove 1 space

	}

	printf("%d", arr[n-1].start);
	for(j=0; j<tot_time - arr[n-1].start; j++)
		printf("  ");
	if(tot_time > 9)
		printf("\b%d", tot_time); // backspace : remove space for two digit time instances
    printf("\n");
}

void fcfs()
{
	system("clear");

	printf("\n\n\tFIRST COME FIRST SERVE\n\n");

	int n, tot_time = 0;
	float waiting = 0, tot_waiting = 0, tot_turn = 0;
	struct job ready[N];

	printf("\n\tNumber of processes: ");
	scanf("%d", &n);

	for(int i=0; i<n; i++)
		ready[i] = read_job();

	fcfs_sort(ready, n);

	for(int i=0; i<n; i++)
	{
		if(i != 0)
		{
			ready[i].waiting_time = (waiting - ready[i].arrival_time);
			ready[i].start = waiting;
		}

		else
		{
			ready[i].waiting_time = waiting;
			ready[i].start = 0;
		}

		ready[i].turn_around = ready[i].waiting_time + ready[i].burstTime;
		waiting += ready[i].burstTime;

		if(i != n-1)
			tot_waiting += waiting;

		tot_turn += ready[i].turn_around;

	}

	for(int k=0; k<n; k++)
		tot_waiting -= ready[k].arrival_time;

	for(int i=0; i<n; i++)
		tot_time += ready[i].burstTime;

	table(ready, tot_waiting, tot_turn, n);
	gantt(ready, n, tot_time);
}

void sjf()
{
	system("clear");

	printf("\n\n\tSHORTEST JOB FIRST\n\n");

	int n, k = 0;
	struct job ready[N], final[N];

	int time=0, tot_time = 0;
	float avgturn = 0, avgwait = 0;
	int min;
	int index;

	printf("\n\tNumber of processes: ");
	scanf("%d", &n);

	for(int i=0; i<n; i++)
		ready[i] = read_job();

	for(int i=0;i<n;i++)
		time += ready[i].burstTime;

	for(int t=0; t<time; )
	{
		min = 9999;

		for(int i=0; i<n; i++)
		{
			if(ready[i].arrival_time <= t && ready[i].burstTime < min && ready[i].flag == 0)
			{
				min = ready[i].burstTime;
				index = i;
			}
		}

		ready[index].flag = 1;
		ready[index].waiting_time = t - ready[index].arrival_time;

		ready[index].start = t;
		final[k++] = ready[index];

		t += ready[index].burstTime;
		ready[index].turn_around = t - ready[index].arrival_time;

		avgturn += ready[index].turn_around;
		avgwait += ready[index].waiting_time;
	}

	for(int i=0; i<n; i++)
		tot_time += ready[i].burstTime;

	table(ready, avgwait, avgturn, n);
	gantt(final, n, tot_time);
}

void srtf()
{
	system("clear");

	printf("\n\n\tSHORTEST REMAINING TIME LEFT\n\n");

	int n;
	struct job ready[N], final[N];

	printf("\n\tNumber of processes: ");
	scanf("%d", &n);

	for(int i=0; i<n; i++)
		ready[i] = read_job();

	int time = 0, tot_time = 0;
	float avgturn = 0, avgwait = 0;
	int min, index, k = 0;
	int count = 0, endtime;

	for(int i=0; i<n; i++)
		time += ready[i].burstTime;

	for(int t=0; count != n; t++)
	{
		min = 9999;

		for(int i=0; i<n; i++)
		{
			if( ready[i].arrival_time <= t && ready[i].remaining < min && ready[i].remaining > 0)
			{
				min = ready[i].remaining;
				index = i;
			}
		}

		if(k != 0 && (final[k-1].pid != ready[index].pid))
		{
			ready[index].start = t;
			final[k++] = ready[index];
		}

		else if(k == 0 && t == 0)
		{	ready[index].start = t;
			final[k++] = ready[index];
		}

		ready[index].remaining -= 1;

		if(ready[index].remaining == 0)
		{
			count++;
			endtime = t+1;
			ready[index].turn_around = endtime - ready[index].arrival_time;
			ready[index].waiting_time = endtime - ready[index].burstTime - ready[index].arrival_time;
		}
	}

	for(int i=0; i<n; i++)
	{
		avgturn += ready[i].turn_around;
		avgwait += ready[i].waiting_time;
	}

	for(int i=0; i<n; i++)
		tot_time += ready[i].burstTime;

	table(ready, avgwait, avgturn, n);
	gantt(final, k, tot_time);
}

//MAIN PROGRAM

int main()
{
	char choice, option;

	system("clear");

	//DISPLAYING THE MENU

	do
	{
		system("clear");

		printf("\n\n\t+----------------------------+\n");
		printf("\t\tMENU OPTIONS\n");
		printf("\t+----------------------------+\n\n");

		printf("\t1. FCFS\n\n");
		printf("\t2. SJF\n\n");
		printf("\t3. EXIT\n\n");

		printf("\n\n\tENTER choice: ");
		scanf(" %c", &choice);

		switch(choice)
		{
			case '1':	fcfs();
					getch();
					break;

			case '2':	system("clear");

						printf("\n\n\t+----------------------------+\n");
						printf("\t\tMENU OPTION FOR SJF\n");
						printf("\t+----------------------------+\n\n");
						printf("\t1. Non pre-emptive SJF\n\n");
						printf("\t2. Pre-emptive SJF\n\n");

						printf("\n\n\tENTER choice: ");
						scanf(" %c", &option);

						switch(option)
						{
							case '1':	sjf();
										break;

							case '2':	srtf();
										break;

							default :	printf("\n\tINVALID OPTION .....!!!!!\n\n");
										getch();
						}

						getch();
						break;

			case '3':	system("clear");
						return 0;
						break;

			default:	printf("\n\tINVALID OPTION .....!!!!!\n\n");
						getch();
		}
	}while(choice!=5);
}
