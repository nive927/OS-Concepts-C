 //cd ../../mnt/d/work/nive/SSN\ College\ Of\ Engineering/all\ SEMESTERS/SEM4/UCS1411\ Operating\ Systems\ Lab/A9

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
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

#define N 25

// To store details of memory structures
struct mnode
{
	char id[N];
	int size;// total size of memory
	int page_size; // page size = frame size
	int num; // no. of frames/pages
	int table[N]; //free frames  table or page table
	int limit; //limited free frames or working size of the table
};

struct mnode process[N];
int ws = 0;

// comparator for quicksort, to verify that there are no duplicates
// and to order the free frames
int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

// Finction to remove duplicates in free frames

void removeDuplicates(struct mnode *physical)
{
	int arr[50], flag, count = 0, i, j;

	for(i=0; i<physical->limit; i++)
	{
		flag =0;

		for(j=i+1; j<physical->limit; j++)
		{
			if(physical->table[i] == physical->table[j])
			{
				flag = 1;
				break;
			}
		}

		if(flag == 0)
			arr[count++] = physical->table[i];
    }

	for(i=0; i<count; i++)
		physical->table[i] = arr[i];

  	physical->limit = count;

	qsort(physical->table, physical->limit, sizeof(int), cmpfunc);
}

// Process Request Function

void process_request(struct mnode *physical)
{
	printf("\n\n\n\tEnter the process requirement(ID, size): ");
	scanf(" %s %d", process[ws].id, &process[ws].size);

	process[ws].page_size = physical->page_size;
	process[ws].num = process[ws].size / process[ws].page_size;

	printf("\n\tProcess has been divided into %d pages", process[ws].num);

	if(process[ws].num <= physical->limit)
	{
		for(int i=0; i<process[ws].num; i++)
			process[ws].table[i] = physical->table[i];

		process[ws].limit = process[ws].num;

		for(int i=0; i+process[ws].num < physical->limit; i++)
			physical->table[i] = physical->table[i+process[ws].num];

		physical->limit -= process[ws].num;

		printf("\n\n\tPAGE TABLE for process %s\n", process[ws].id);
		for(int i=0; i<process[ws].num; i++)
			printf("\n\tPage: %d Frame: %d", i, process[ws].table[i]);

		ws++;
	}

	else
		printf("\n\tFREE FRAMES INSUFFICIENT !");
}

// Function to display free frames

void free_frame_display(struct mnode physical)
{
	printf("\n\tTotal Number of frames: %d", physical.num);

	printf("\n\n\tNumber of free frames: %d", physical.limit);

	printf("\n\n\tFree Frames List:");

	for(int i=0; i<physical.limit; i++)
	printf(" %d", physical.table[i]);
}

// Function to display page table of all processes with allocated frames

void process_display_all()
{
	if(ws == 0)
	{
		printf("\n\n\tPAGE TABLES UNAVAILABLE !");
		return;
	}
	for(int i=0; i<ws; i++)
	{
		printf("\n\n\tPAGE TABLE for process %s\n", process[i].id);
		for(int j=0; j<process[i].num; j++)
			printf("\n\tPage: %d Frame: %d", j, process[i].table[j]);
	}
}

// Function to deallocate frames from a process

void deallocation(struct mnode *physical)
{
	char id[N];
	int index, found = 0;

	index = physical->limit;

	printf("\n\n\n\tEnter the process ID for de-allocation: ");
	scanf(" %s", id);

	for(int i=0; i<ws; i++)
	{
		if(strcmp(process[i].id, id) == 0)
		{
			found = 1;
			for(int j=0; j<process[i].num; j++)
				physical->table[index++] = process[i].table[j];

			for(int k = i; k<ws-1; k++)
				process[k] = process[k+1];

			ws--;
			physical->limit = index;
			printf("\n\n\tMemory DEALLOCATED !");
			break;
		}

	}

	if(found == 0)
		printf("\n\tINVALID ID !");

}

// MAIN PROGRAM

int main()
{
	char choice;

	struct mnode physical;

	strcpy(physical.id, "Physical");

	system("clear");

	printf("\n\n\t+-------------------------------------------+\n");
	printf("\t\tPHYSICAL MEMORY PARTITIONING\n");
	printf("\t+-------------------------------------------+\n\n");

	printf("\n\tEnter the physical memory size(in KB): ");
	scanf(" %d", &physical.size);
	printf("\n\tEnter the page size(in KB): ");
	scanf(" %d", &physical.page_size);

	physical.num = physical.size / physical.page_size;

	srand(time(0));

	physical.limit = (rand() % physical.num);

	if(physical.limit < 5)
		physical.limit += 5;

	// Formula to get a random number within a range
	//rand() % (upper - lower + 1)) + lower;

	for(int i=0; i<physical.limit ;i++)
		physical.table[i] = (rand() % physical.num);

	removeDuplicates(&physical);

	free_frame_display(physical);

	getch();

	//DISPLAYING THE MENU

	do
	{
		system("clear");

		printf("\n\n\t+----------------------------+\n");
		printf("\t\tMENU OPTIONS\n");
		printf("\t+----------------------------+\n\n");

		printf("\t1. PROCESS REQUEST\n\n");
		printf("\t2. DEALLOCATION\n\n");
		printf("\t3. PAGE TABLE DISPLAY FOR ALL PROCESSES\n\n");
		printf("\t4. FREE FRAME LIST DISPLAY\n\n");
		printf("\t5. EXIT\n\n");

		printf("\n\n\tENTER choice: ");
		scanf(" %c", &choice);

		// For convenience the free frames are always displayed.
		// Just comment them out if you don't want them.

		switch(choice)
		{
			case '1':	//free_frame_display(physical);
						process_request(&physical);
						getch();
						break;

			case '2':	//free_frame_display(physical);
						deallocation(&physical);
						getch();
						break;

			case '3':	//free_frame_display(physical);
						process_display_all();
						getch();
						break;

			case '4':
						free_frame_display(physical);
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

