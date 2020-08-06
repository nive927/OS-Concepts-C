#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define N 10

int P = 5;
int R = 3;

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

// Function to read data
void read_data(int process[], char resource[], int available[], int maximum[][N], int allocated[][N])
{
	printf("\n\n\tREAD DATA\n\n");

	printf("\n\tEnter the number of processes: ");
	scanf("%d", &P);

	printf("\n\tProcess IDs\n\n");
	for(int i=0; i<P; i++)
	{
		printf("\tEnter process ID: P");
		scanf("%d", &process[i]);
	}
	printf("\n\tEnter the number of resources:");
	scanf(" %d", &R);

	printf("\n\tResource Details\n\n");
	for(int i=0; i<R; i++)
	{
		printf("\tEnter resource ID: ");
		scanf(" %c", &resource[i]);

		printf("\tNumber of available instances: ");
		scanf("%d", &available[i]);
	}

	printf("\n\tMaximum Requirement\n");
	for(int i=0; i<P; i++)
	{
		printf("\n\tEnter maximum requirement for P%d:\n",process[i]);

		for(int j=0; j<R; j++)
		{
			printf("\tFor resource %c: ", resource[j]);
			scanf("%d", &maximum[i][j]);
		}
	}

	printf("\n\tAllocated\n");
	for(int i=0; i<P; i++)
	{
		printf("\n\tEnter allocated instances for P%d:\n",process[i]);

		for(int j=0; j<R; j++)
		{
			printf("\tFor resource %c: ", resource[j]);
			scanf("%d", &allocated[i][j]);
		}
	}
}

// Function to print the data in the form of a table
// NOTE: This formatting works only for a max of 4 no. of resources
void print_table(int process[], char resource[], int available[], int maximum[][N], int allocated[][N], int need[][N])
{
	// For reference, in case the table formatting requires change
	// 7 12 12 12 12


	printf("\n\n\tPRINT DATA\n\n");

	printf("\n\t+-------+------------+------------+------------+------------+");
	printf("\n\t|  PID  | Allocation |   Maximum  |    Need    |  Available |");
  	printf("\n\t+-------+------------+------------+------------+------------+");

	// To print the Resource Names
	printf("\n\t|       |");
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<R; j++)
			printf("%*c", 12/R, resource[j]);
		printf("|");
	}

	printf("\n\t+-------+------------+------------+------------+------------+");

	// To print the details of each process
	for(int i=0; i<P; i++)
	{
		printf("\n\t|  P%d   |", process[i]);

		for(int j=0; j<R; j++)
			printf("%*d", 12/R, allocated[i][j]);
		printf("|");

		for(int j=0; j<R; j++)
			printf("%*d", 12/R, maximum[i][j]);
		printf("|");

		for(int j=0; j<R; j++)
			printf("%*d", 12/R, need[i][j]);
		printf("|");

		for(int j=0; j<R; j++)
		{
			if(i == 0)
				printf("%*d", 12/R, available[j]);
			else
			{
				printf("            ");
				break;
			}
		}
		printf("|");

		printf("\n\t+-------+------------+------------+------------+------------+");
	}
}

// Function to find the need of each process
void calculateNeed(int need[][N], int maximum[][N], int allocated[][N])
{
	// Calculating Need of each P
	for (int i = 0 ; i < P ; i++)
		for (int j = 0 ; j < R ; j++)
			need[i][j] = maximum[i][j] - allocated[i][j];
}

// Safety Algorithm Function
int isSafe(int available[], int maximum[][N], int allocated[][N], int need[][N])
{
	int finish[N];
	int safeSeq[N];
	int work[N];
	int flag;
	int index ;

	// Marking all processes as unfinishished
	for(int i=0; i<P; i++)
		finish[i] = 0;

	//Calculating work matrix
	for (int i=0; i<R ; i++)
		work[i] = available[i];

	index = 0;
	for (int p=0; p<P; p++)
	{
		for (int i=0; i<P; i++)
		{
			if (finish[i] == 0)
			{
				flag = 0;

				for (int j=0; j<R; j++)
				{
					if (need[i][j] > work[j])
					{
						flag = 1;
						break;
					}
				}

				if (flag == 0)
				{
					safeSeq[index++] = i;
					for (int k=0; k<R; k++)
						work[k] += allocated[i][k];
					finish[i] = 1;
				}
			}
		}

		if (flag == 1)
		{
			printf("\n\n\tUNSAFE: System is not in safe state !");
			return 0;
		}
    }

	printf("\n\n\tSAFE STATE: System is in safe state !");
	printf("\n\n\tSAFETY SEQUENCE: ");
	printf("< ");
	for (int i = 0; i < P ; i++)
		printf("P%d, ", safeSeq[i]);

	printf("\b\b >");
	printf("\n");

	return 1;
}

// MAIN PROGRAM

int main()
{
	char choice;

	int process[N];
	char resource[N];

	int available[N];
	int maximum[N][N];
	int allocated[N][N];

	int need[N][N];
	// Default problem details used for testing
	// This is provided to quickly check if the program works and for debugging purposses

	// If read data isn't chosen, this is the problem solved
	// When data is given by the user, user-defined problem is solved

	// Process IDs
	process[0] = 0;
	process[1] = 1;
	process[2] = 2;
	process[3] = 3;
	process[4] = 4;

	// Resources IDs
	resource[0] = 'A';
	resource[1] = 'B';
	resource[2] = 'C';

	// Available instances of resources
	available[0] = 3;
	available[1] = 3;
	available[2] = 2;

	// Maximum R that can be allocated
	// to processes
	maximum[0][0] = 7;
	maximum[0][1] = 5;
	maximum[0][2] = 3;

	maximum[1][0] = 3;
	maximum[1][1] = 2;
	maximum[1][2] = 2;

	maximum[2][0] = 9;
	maximum[2][1] = 0;
	maximum[2][2] = 2;

	maximum[3][0] = 2;
	maximum[3][1] = 2;
	maximum[3][2] = 2;

	maximum[4][0] = 4;
	maximum[4][1] = 3;
	maximum[4][2] = 3 ;

	// Resources allocated to processes
	allocated[0][0] = 0;
	allocated[0][1] = 1;
	allocated[0][2] = 0;

	allocated[1][0] = 2;
	allocated[1][1] = 0;
	allocated[1][2] = 0;

	allocated[2][0] = 3;
	allocated[2][1] = 0;
	allocated[2][2] = 2;

	allocated[3][0] = 2;
	allocated[3][1] = 1;
	allocated[3][2] = 1;

	allocated[4][0] = 0;
	allocated[4][1] = 0;
	allocated[4][2] = 2;

	system("clear");

	//DISPLAYING THE MENU

	do
	{
		system("clear");

		printf("\n\n\t+----------------------------+\n");
		printf("\t\tMENU OPTIONS\n");
		printf("\t+----------------------------+\n\n");

		printf("\t1. READ DATA\n\n");
		printf("\t2. PRINT DATA\n\n");
		printf("\t3. SAFETY SEQUENCE\n\n");
		printf("\t4. EXIT\n\n");

		printf("\n\n\tENTER choice: ");
		scanf(" %c", &choice);

		switch(choice)
		{
			case '1':	system("clear");
						read_data(process, resource, available, maximum, allocated);

						getch();
						break;

			case '2':
						// Calculating need matrix
						calculateNeed(need, maximum, allocated);
						print_table(process, resource, available, maximum, allocated, need);
						getch();
						break;

			case '3':	calculateNeed(need, maximum, allocated);
						isSafe(available, maximum, allocated, need);

						getch();
						break;

			case '4':	system("clear");
						return 0;
						break;

			default:	printf("\n\tINVALID OPTION .....!!!!!\n\n");
						getch();
		}
	}while(choice!=4);
}
