
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
};

struct job read_job()
{
	struct job process;

	printf("\n\n\tProcess ID: P");
	scanf("%d", &process.pid);
	printf("\tArrival Time: ");
	scanf("%d", &process.arrival_time);
	printf("\tBurst Time: ");
	scanf("%f", &process.burstTime);

	return process;
}

void insertJob(struct job arr[], struct job val, int pos, int *n)
{

	for(int i=*n-1; i>=pos-1; i--)
		arr[i+1] = arr[i];

	arr[pos-1] = val;

	*n = *n + 1;

}

void deleteJob(struct job arr[], int pos, int *n)
{
	for(int i=pos-1; i<*n; i++)
		arr[i] = arr[i+1];

	*n = *n -1;

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

void sjf_sort(struct job a[], int n)
{
	int i, j;
	struct job small;

	for(i=1; i<n; i++)
	{
		small = a[i];
		j = i-1;

		while((j >= 0) && (small.burstTime < a[j].burstTime))
		{
			a[j+1] = a[j];
			j--;
		}

		a[j+1] = small;
	}
}

void table(struct job ready[], float tot_waiting, float tot_turn, int n)
{
	printf("\n\n\tPROCESS CHART");

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

void fcfs()
{
	system("clear");

	int n;
	float waiting = 0, tot_waiting = 0, tot_turn = 0;
	struct job pool[N], ready[N];

	printf("\n\tNumber of processes: ");
	scanf("%d", &n);

	for(int i=0; i<n; i++)
	{	pool[i] = read_job();
		ready[i] = pool[i];
	}

	fcfs_sort(ready, n);

	for(int i=0; i<n; i++)
	{
		if(i != 0)
			ready[i].waiting_time = (waiting - ready[i].arrival_time);
		else
			ready[i].waiting_time = waiting;

		ready[i].turn_around = ready[i].waiting_time + ready[i].burstTime;
		waiting += ready[i].burstTime;

		if(i != n-1)
			tot_waiting += waiting;

		tot_turn += ready[i].turn_around;

	}

	for(int k=0; k<n; k++)
		tot_waiting -= ready[k].arrival_time;

	table(ready, tot_waiting, tot_turn, n);
}

void sjf()
{
	system("clear");

	int n;
	float waiting = 0, tot_waiting = 0, tot_turn = 0;
	struct job pool[N], ready[N];

	printf("\n\tNumber of processes: ");
	scanf("%d", &n);

	for(int i=0; i<n; i++)
	{	pool[i] = read_job();
		ready[i] = pool[i];
	}

	int arrival, ws = n, j = 0, k = 0;
	struct job temp[N];

	fcfs_sort(pool, n);

	for(int i=0; i<n; i++)
		display_job(pool[i]);

	arrival = pool[0].arrival_time;

	while(arrival == pool[j].arrival_time)
	{
		temp[k++] = pool[j];
		deleteJob(pool, j+1, &ws);
		j++;
	}

	for(int i=0; i<ws; i++)
	display_job(pool[i]);

	sjf_sort(ready, n);

	for(int i=0; i<n; i++)
	{
		if(i != 0)
			ready[i].waiting_time = (waiting - ready[i].arrival_time);
		else
			ready[i].waiting_time = waiting;

		ready[i].turn_around = ready[i].waiting_time + ready[i].burstTime;
		waiting += ready[i].burstTime;

		if(i != n-1)
			tot_waiting += waiting;

		tot_turn += ready[i].turn_around;

	}

	for(int k=0; k<n; k++)
		tot_waiting -= ready[k].arrival_time;

	table(ready, tot_waiting, tot_turn, n);
}

void srtf()
{
	system("clear");

	int n;
	float tot_waiting = 0, tot_turn = 0;
	struct job pool[N], ready[N];

	printf("\n\tNumber of processes: ");
	scanf("%d", &n);

	for(int i=0; i<n; i++)
	{
		pool[i] = read_job();
		ready[i] = pool[i];
	}

	sjf_sort(ready, n);

	for(int i=0; i<n; i++)
	{
		ready[i].waiting_time = tot_waiting;
		ready[i].turn_around = ready[i].waiting_time + ready[i].burstTime;
		tot_waiting += ready[i].burstTime;
		tot_turn += ready[i].turn_around;
	}

	tot_waiting -= ready[n-1].burstTime;

	table(ready, tot_waiting, tot_turn, n);
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
				case '1':		fcfs();
							getch();
							break;

				case '2':		system("clear");

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
								case '2':

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
