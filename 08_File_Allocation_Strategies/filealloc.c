//cd ../../mnt/d/work/nive/SSN\ College\ Of\ Engineering/all\ SEMESTERS/SEM4/UCS1411\ Operating\ Systems\ Lab/A12

#include "SLL.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

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

struct dir1
{
	char fname[N];
	int startBlock;
	int length;
};

struct dir2
{
	char fname[N];
	struct node *startBlock;
	struct node *endBlock;
};

struct dir3
{
	char fname[N];
	struct node *indexedBlock;
};

int i, j, k;
int blocks[100];

// number of blocks & number of frames
int n, nf;
int blockSize;
int memSize;

struct file files[N];
struct node *memSLL;

struct dir1 d1[N];
int nd1 = 0;

struct dir2 d2[N];
int nd2 = 0;

struct dir3 d3[N];
int nd3 = 0;

void initializeSLL()
{
	memSLL = NULL;

	for(i=0; i<n; i++)
	{
		append(&memSLL, i+1);
		blocks[i] = i+1;
	}

}

void read_data()
{
	printf("\n\n\tMain Memory size(in KB): ");
	scanf("%d", &memSize);

	printf("\n\tSize of each block on the disk: ");
	scanf("%d", &blockSize);

	// Dividing main memory into equal sized blocks
	n = memSize / blockSize;

	//initializeSLL();

	/*
	display(&memSLL);
	*/

	printf("\n\tNumber of files to be allocated: ");
	scanf("%d", &nf);

	printf("\n");

	for(i=0; i<nf; i++)
	{
		printf("\tEnter the name and size of file %d(in KB): ",(i+1));
		scanf(" %s %d", files[i].name, &files[i].size);

		if(files[i].size % blockSize == 0)
			files[i].numBlocks = files[i].size / blockSize;

		else
			files[i].numBlocks = (files[i].size / blockSize) + 1;
	}

	nd1 = 0;
	nd2 = 0;
	nd3 = 0;
}

void print1()
{
	printf("\n\n\t---------------------------------------------------------------");
	printf("\n\t\tFILENAME\tSTART BLOCK ID\t\tLENGTH");
	printf("\n\t---------------------------------------------------------------");

	for(int p=0; p<nd1; p++)
	{
		printf("\n\t\t%s\t\t%d\t\t\t%d", d1[p].fname, d1[p].startBlock, d1[p].length);
	}

	printf("\n\t---------------------------------------------------------------");
}

void print2()
{
	struct node *current;

	printf("\n\n\t---------------------------------------------------------------");
	printf("\n\t\tFILENAME\tSTART BLOCK ID\t\tEND BLOCK ID");
	printf("\n\t---------------------------------------------------------------");

	for(int p=0; p<nd2; p++)
	{
		printf("\n\t\t%s\t\t%d\t\t\t%d", d2[p].fname, d2[p].startBlock->data.bid, d2[p].endBlock->data.bid);
	}

	printf("\n\t---------------------------------------------------------------");

	printf("\n\n\n\n\tINDIVIDUAL FILE LINKED LISTS\n");

	for(int p=0; p<nd2; p++)
	{
		printf("\n\t%s:", d2[p].fname);

		 current = d2[p].startBlock;

		while (current != NULL)
		{
			printf(" Data Block ID: %d ->", current->data.bid);
			current = current->fnext;
    	}

    	printf("\b\b  \n");

	}

}

void print3()
{
	struct node *current;

	printf("\n\n\t---------------------------------------------------------------");
	printf("\n\t\tFILENAME\tINDEXED BLOCK");
	printf("\n\t---------------------------------------------------------------");

	for(int p=0; p<nd3; p++)
	{
		printf("\n\t\t%s\t\t%d", d3[p].fname, d3[p].indexedBlock->data.bid);
	}

	printf("\n\t---------------------------------------------------------------");

	printf("\n\n\n\n\tINDEX TABLE FOR EACH FILE");
	printf("\n\n\t---------------------------------------------------------------");
	printf("\n\tFILENAME\tBLOCK INDEXED");
	printf("\n\t---------------------------------------------------------------");

	for(int p=0; p<nd3; p++)
	{
		printf("\n\n\t%s", d3[p].fname);

		 current = d3[p].indexedBlock;

		for(int h=0; h < files[p].numBlocks - 1; h++)
			printf("\t\tData Block %d\n\t", current->data.fileBlockTable[h]);

		if(files[p].numBlocks == 1)
			printf("\t\tEMPTY\n\t");

	}

	printf("\n\t---------------------------------------------------------------");

}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// Fisher-Yates Shuffle Algorithm
// A function to generate a random permutation of arr[]
void randomize(int arr[], int n )
{
    srand(time(0));

    for(int i=n-1; i>0; i--)
    {
        int j = rand() % (i+1);
        swap(&arr[i], &arr[j]);
    }
}

void contiguous()
{
	initializeSLL();

	int count, flag;
	struct node *temp, *current;

	for(i=0; i<nf; i++)
	{
		// Randomly generated start index for blocks
		randomize(blocks, n);
		flag = 0;

		for(j=0; j<n; j++)
		{
			temp = memSLL;

			// Locating the block in the LL
			for(k=1; k<blocks[j]; k++)
				temp = temp->next;

			count = 1;
			current = temp;

			// Checking if there are contiguous free spaces
			while (current != NULL)
			{
				if(strcmp(current->data.f.name, "free") == 0)
				{
					current = current->next;
					count++;
				}

				else
					break;
			}

			if(count >= files[i].numBlocks)
			{
				current = temp;

				strcpy(d1[nd1].fname, files[i].name);
				d1[nd1].startBlock = current->data.bid;
				d1[nd1++].length = files[i].numBlocks;

				k = 1;

				while (k <= files[i].numBlocks)
				{
					current->data.f = files[i];
					current = current->next;
					k++;
				}

				flag = 1;
				break;
			}
		}

		if(flag == 0)
			printf("\n\tUNABLE to ALLOCATE memory for %s in CONTIGUOUS TECHNIQUE!", files[i].name);
	}

	printf("\n\n\tDIRECTORY STRUCTURE");
	print1();
}

void linked()
{
	initializeSLL();

	int count, flag;
	struct node *temp, *current;

	for(i=0; i<nf; i++)
	{
		// Randomly generated start index for blocks
		randomize(blocks, n);
		flag = 0;
		count = 0;

		for(j=0; j<n; j++)
		{
			temp = memSLL;

			// Locating the block in the LL
			for(k=1; k<blocks[j]; k++)
				temp = temp->next;

			current = temp;

			// Checking if block is free
			if(strcmp(current->data.f.name, "free") == 0 && count < files[i].numBlocks)
			{

				if(count == 0)
				{
					current->data.f = files[i];
					current->fnext = NULL;

					strcpy(d2[nd2].fname, files[i].name);
					d2[nd2].startBlock = current;

					count++;

					if(count == files[i].numBlocks)
					{
						d2[nd2++].endBlock = current;
						flag = 1;
						break;
					}

				}

				else
				{
					current->data.f = files[i];
					current->fnext  = NULL;

					struct node *last = d2[nd2].startBlock;

					while (last->fnext != NULL)
						last = last->fnext;

    				last->fnext = current;

    				count++;

    				if(count == files[i].numBlocks)
    				{
    					d2[nd2++].endBlock = current;
    					flag = 1;
    					break;
					}
				}
			}

		}

		if(flag == 0)
			printf("\n\tUNABLE to ALLOCATE memory for %s in LINKED TECHNIQUE!", files[i].name);
	}

	printf("\n\n\tDIRECTORY STRUCTURE");
	print2();
}

void indexed()
{
	initializeSLL();

	int count, flag, index;
	struct node *temp, *current;

	for(i=0; i<nf; i++)
	{
		// Randomly generated start index for blocks
		randomize(blocks, n);
		flag = 0;
		count = 0;
		index = 0;

		for(j=0; j<n; j++)
		{
			temp = memSLL;

			// Locating the block in the LL
			for(k=1; k<blocks[j]; k++)
				temp = temp->next;

			current = temp;

			// Checking if block is free
			if(strcmp(current->data.f.name, "free") == 0 && count < files[i].numBlocks)
			{

				if(count == 0)
				{
					current->data.f = files[i];

					strcpy(d3[nd3].fname, files[i].name);
					d3[nd3].indexedBlock = current;

					count++;

					if(count == files[i].numBlocks)
					{
						nd3++;
						flag = 1;
						break;
					}

				}

				else
				{
					current->data.f = files[i];
					d3[nd3].indexedBlock->data.fileBlockTable[index++] = current->data.bid;

					count++;

					if(count == files[i].numBlocks)
					{
						nd3++;
						flag = 1;
						break;
					}
				}
			}

		}

		if(flag == 0)
			printf("\n\tUNABLE to ALLOCATE memory for %s in INDEXED TECHNIQUE!", files[i].name);
	}

	printf("\n\n\tDIRECTORY STRUCTURE");
	print3();
}

// MAIN PROGRAM

int main()
{
	char choice;

	//DISPLAYING THE MENU

	do
	{
		system("clear");

		printf("\n\n\t+-----------------------------------------+\n");
		printf("\t\tFILE ALLOCATION TECHNIQUES\n");
		printf("\t+-----------------------------------------+\n\n");

		printf("\t1. READ DATA\n\n");
		printf("\t2. CONTIGUOUS\n\n");
		printf("\t3. LINKED\n\n");
		printf("\t4. INDEXED\n\n");
		printf("\t5. EXIT\n\n");

		printf("\n\n\tENTER choice: ");
		scanf(" %c", &choice);

		switch(choice)
		{
			case '1':	read_data();

						getch();
						break;

			case '2': 	contiguous();

						getch();
						break;

			case '3':	linked();

						getch();
						break;

			case '4':	indexed();

						getch();
						break;

			case '5':	system("clear");
						return 0;
						break;

			default:	printf("\n\tINVALID OPTION .....!!!!!\n\n");
						getch();
		}
	}while(choice!=4);
}
