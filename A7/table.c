#include<stdio.h>

// Function to print the data in the form of a table with column separators for resources
void print_table1(int process[], char resource[], int available[], int maximum[][N], int allocated[][N], int need[][N])
{
	//7 12 12 12 12

	printf("\n\n\tPRINT DATA\n\n");

	printf("\n\t+-------+------------+------------+------------+------------+");
	printf("\n\t|  PID  | Allocation |   Maximum  |    Need    |  Available |");
    printf("\n\t+-------+------------+------------+------------+------------+");

	printf("\n\t|       ");
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<R; j++)
			printf("|%2c ", resource[j]);
		printf(" ");
	}

	printf("|");

	printf("\n\t+-------+------------+------------+------------+------------+");

	for(int i=0; i<P; i++)
	{
		printf("\n\t|  P%d   ", process[i]);

		for(int j=0; j<R; j++)
			printf("|%2d ", allocated[i][j]);
		printf(" ");

		for(int j=0; j<R; j++)
			printf("|%2d ", maximum[i][j]);
		printf(" ");

		for(int j=0; j<R; j++)
			printf("|%2d ", need[i][j]);
		printf(" ");

		for(int j=0; j<R; j++)
			printf("|%2d ", available[j]);
		printf(" ");
		printf("|");

		printf("\n\t+-------+------------+------------+------------+------------+");
	}
}

// Function to print the data in the form of a table w/o column separators for resources
void print_table(int process[], char resource[], int available[], int maximum[][N], int allocated[][N], int need[][N])
{
	// FOr reference, in case the table formatting requires change
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
			printf(" %2c ", resource[j]);
		printf("|");
	}

	printf("\n\t+-------+------------+------------+------------+------------+");

	// To print the details of each process
	for(int i=0; i<P; i++)
	{
		printf("\n\t|  P%d   |", process[i]);

		for(int j=0; j<R; j++)
			printf(" %2d ", allocated[i][j]);
		printf("|");

		for(int j=0; j<R; j++)
			printf(" %2d ", maximum[i][j]);
		printf("|");

		for(int j=0; j<R; j++)
			printf(" %2d ", need[i][j]);
		printf("|");

		for(int j=0; j<R; j++)
		{
			if(i == 0)
				printf(" %2d ", available[j]);
			else
				printf("    ");
		}
		printf("|");

		printf("\n\t+-------+------------+------------+------------+------------+");
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
