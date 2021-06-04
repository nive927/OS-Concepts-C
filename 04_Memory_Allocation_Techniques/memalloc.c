 //cd ../../mnt/d/work/nive/SSN\ College\ Of\ Engineering/all\ SEMESTERS/SEM4/UCS1411\ Operating\ Systems\ Lab/A8

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

// Structure to store memory details
struct mnode
{
	int start;
	int end;
	int size;
	char status[N];
};

struct mnode freepool[N];
struct mnode allocated[N];
struct mnode physical[N];

int nf = 0;
int na = 0;
int np = 0;

 struct mnode original[N];
 int n;

struct process
{
	char id[N];
	int size;
};

struct process p;

void read_partitions()
{
	for(int i=0; i<n; i++)
	{
		printf("\n\tEnter Start Address and End Address of Partition %d: ", (i+1));
		scanf("%d %d", &freepool[i].start, &freepool[i].end);

		freepool[i].size = freepool[i].end - freepool[i].start;

		strcpy(freepool[i].status, "H");

		physical[i] = freepool[i];
		original[i] = freepool[i];
	}

	nf = n;
	np = n;

}

// To reset freepool, allocated and physical to initial representation
void reset()
{
    for(int i=0; i<n; i++)
    {
        freepool[i] = original[i];
        physical[i] = original[i];
    }

    nf = n;
    np = n;
    na = 0;

}

int max_size(struct mnode arr[], int ws)
{
	int max = arr[0].size;

	for(int k=1; k<ws; k++)
		if(arr[k].size > max)
			max = arr[k].size;

	return max;
}

int min_size(struct mnode arr[], int ws)
{
	int min = arr[0].size;

	for(int k=1; k<ws; k++)
		if(arr[k].size < min)
			min = arr[k].size;

	return min;
}

void print_mem(struct mnode arr[], int ws)
{
	int i, j;
	int node_width[N];

	int oldRange, oldMax, oldMin, newRange, newMax, newMin;

	/* Formula for changing the range to fit screen size

		oldRange = oldMax - oldMin
		newRange = newMax - newMin

		newValue = ((oldValue - oldMin) * newRange / oldRange) + newMin
	*/

	oldMin = min_size(arr, ws);
	oldMax = max_size(arr, ws);

	oldRange = oldMax - oldMin;

	// To adjust the display in case of distortion, modify these values
	newMin = 8;
	newMax = 20;

	newRange = newMax - newMin;

	// To prevent division by zero
	if(oldRange == 0)
		oldRange = 1;

	for(i=0; i<ws; i++)
		node_width[i] = ((arr[i].size - oldMin) * newRange / oldRange) + newMin;

	/* FR DE-BUGGING
	for(i = 0; i<ws; i++)
		printf("\n\tnode_width %d: %d", (i+1), node_width[i]);
	*/

	// printing the TOP BAR
	printf("\n\t ");
	for(i=0; i<ws; i++)
	{
		for(j=0; j<node_width[i]; j++)
			printf("-");
		printf(" ");
	}


	// printing the STATUS in the middle
	printf("\n\t|");
	for(i=0; i<ws; i++)
	{
		printf("%s", arr[i].status);
		for(j=0; j<((node_width[i])-strlen(arr[i].status)); j++)
			printf(" ");

		printf("|");
	}

	printf("\n\t ");

	// printing the bottom bar
	for(i=0; i<ws; i++)
	{
		for(j=0; j<node_width[i]; j++)
			printf("-");
		printf(" ");
	}

	printf("\n\t");

	// printing the bytes line
	for(i=0; i<ws; i++)
	{
		printf("%d", arr[i].start);
		for(j=0; j<node_width[i]; j++)
			printf(" ");
		if(arr[i].start > 9 && arr[i].start < 100)
			printf("\b"); // backspace : remove 1 space
		else if(arr[i].start > 99 && arr[i].start < 1000)
			printf("\b\b"); // backspace : remove 2 spaces
		else if(arr[i].start > 999 && arr[i].start < 10000)
			printf("\b\b\b"); // backspace : remove 3 spaces
	}

	if(arr[ws-1].end > 9 && arr[ws-1].end < 100)
		printf("\b%d", arr[ws-1].end); // backspace : remove space for two digit bytes
	else if(arr[ws-1].end > 99 && arr[ws-1].end < 1000)
		printf("\b\b%d", arr[ws-1].end); // backspace : remove space for three digit bytes
	else if(arr[ws-1].end > 999 && arr[ws-1].end < 10000)
		printf("\b\b\b%d", arr[ws-1].end); // backspace : remove space for four digit bytes
	printf("\n");

}

void read_process()
{
	printf("\n\tEnter process id: ");
	scanf(" %s", p.id);
	printf("\tEnter process size: ");
	scanf(" %d", &p.size);
}

// comparator for quicksort
int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

void coalesce_holes()
{
 struct mnode temp;
 int index;

 for(int i=0; i<np-1; i++)
 {
	 for(int j=0; j<np-i-1; j++)
	 {
		 if(strcmp(physical[j].status, "H") == 0 && strcmp(physical[j+1].status, "H") != 0)
		 {
			 physical[j+1].start = physical[j].start;
			 physical[j].end = physical[j+1].end;
			 physical[j+1].end = physical[j+1].start + physical[j+1].size;
			 physical[j].start = physical[j+1].end;

			 temp = physical[j];
			 physical[j] = physical[j+1];
			 physical[j+1] = temp;
		 }
	 }
 }

 for(int i=0; i<np; i++)
 {
	 if(strcmp(physical[i].status, "H") == 0)
	 {
		 index = i;
		 break;
	 }
 }

 physical[index].end = physical[np-1].end;
 np = index + 1;

 for(int i=0; i<np; i++)
 {
     physical[i].size = physical[i].end - physical[i].start;
 }

 freepool[0] = physical[index];
 nf = 1;

 for(int i=0; i<na; i++)
 {
	 for(int j=0; j<np; j++)
	 {
		 if(strcmp(allocated[i].status, physical[j].status) == 0)
		 {
			 allocated[i] = physical[j];
			 break;
		 }
	 }
 }

    printf("\n\tCoalescing of Holes COMPLETED !!");

}

void allocate(char algo[])
{
	int index;
	int flag = 0;

	// For best-fit and worst-fit
	int sizes[N];

	for(int h=0; h<nf; h++)
		sizes[h] = freepool[h].size;

	qsort(sizes, nf, sizeof(int), cmpfunc);

	int pos;

	read_process();

	if(strcmp(algo, "FIRST FIT ALLOCATION") == 0)
	{
		for(int i=0; i<nf; i++)
		{
			if(strcmp(freepool[i].status, "H") == 0 && freepool[i].size >= p.size)
			{
				allocated[na].start = freepool[i].start;
				allocated[na].end = allocated[na].start + p.size;
				allocated[na].size = p.size;
				strcpy(allocated[na].status, p.id);

				freepool[i].start = allocated[na].end;
				freepool[i].size -= p.size;

				for(int k=0; k<np; k++)
					if(physical[k].start == allocated[na].start)
						index = k;

				for(int j=np-1; j>=index; j--)
					physical[j+1] = physical[j];

				physical[index] = allocated[na];
				physical[index+1] = freepool[i];

				na++;
				np++;

				printf("\n\tMemory ALLOCATED for %s !", p.id);
				flag = 1;
				break;
			}
		}

		if(flag == 0)
		{
		    printf("\n\tMEMEMORY UNAVAILABLE FOR PROCESS %s !", p.id);
		    printf("\n\tTry OPTION 4 coalescing of holes - COMPACTION !");
		}
	}

	else if(strcmp(algo, "BEST FIT ALLOCATION") == 0) {
        for (int i = 0; i < nf; i++) {
            if (sizes[i] >= p.size) {
                for (int k = 0; k < nf; k++)
                    if (freepool[k].size == sizes[i])
                        pos = k;

                if (strcmp(freepool[pos].status, "H") == 0) {

                    allocated[na].start = freepool[pos].start;
                    allocated[na].end = allocated[na].start + p.size;
                    allocated[na].size = p.size;
                    strcpy(allocated[na].status, p.id);

                    freepool[pos].start = allocated[na].end;
                    freepool[pos].size -= p.size;

                    for (int k = 0; k < np; k++)
                        if (physical[k].start == allocated[na].start)
                            index = k;

                    for (int j = np - 1; j >= index; j--)
                        physical[j + 1] = physical[j];

                    physical[index] = allocated[na];
                    physical[index + 1] = freepool[pos];

                    na++;
                    np++;

                    printf("\n\tMemory ALLOCATED for %s !", p.id);
                    flag = 1;
                    break;
                }
            }
        }

        if (flag == 0)
        {
            printf("\n\tMEMEMORY UNAVAILABLE FOR PROCESS %s !", p.id);
            printf("\n\tTry OPTION 4 coalescing of holes - COMPACTION !");
        }
    }

	else if(strcmp(algo, "WORST FIT ALLOCATION") == 0) {
        for (int i = nf - 1; i >= 0; i--) {
            if (sizes[i] >= p.size) {
                for (int k = 0; k < nf; k++)
                    if (freepool[k].size == sizes[i])
                        pos = k;

                if (strcmp(freepool[pos].status, "H") == 0) {

                    allocated[na].start = freepool[pos].start;
                    allocated[na].end = allocated[na].start + p.size;
                    allocated[na].size = p.size;
                    strcpy(allocated[na].status, p.id);

                    freepool[pos].start = allocated[na].end;
                    freepool[pos].size -= p.size;

                    for (int k = 0; k < np; k++)
                        if (physical[k].start == allocated[na].start)
                            index = k;

                    for (int j = np - 1; j >= index; j--)
                        physical[j + 1] = physical[j];

                    physical[index] = allocated[na];
                    physical[index + 1] = freepool[pos];

                    na++;
                    np++;

                    printf("\n\tMemory ALLOCATED for %s !", p.id);
                    flag = 1;
                    break;
                }
            }
        }

        if (flag == 0)
        {
            printf("\n\tMEMEMORY UNAVAILABLE FOR PROCESS %s !", p.id);
            printf("\n\tTry OPTION 4 coalescing of holes - COMPACTION !");
        }
    }
}

void deallocate()
{
    printf("\n\tEnter the process ID: ");
    scanf(" %s", p.id);

    int index = -1;

    for(int i=0; i<np; i++)
    {
        if(strcmp(physical[i].status, p.id) == 0)
        {
            strcpy(physical[i].status, "H");
            index = i;
            break;
        }
    }

    if(index == -1)
    {
        printf("\n\tINVALID process ID !");
        return;
    }

    for(int i=0; i<na; i++)
    {
        if (strcmp(allocated[i].status, p.id) == 0)
        {
            index = i;
            break;
        }
    }

    freepool[nf] = allocated[index];
    strcpy(freepool[nf].status, "H");

    nf++;

    for(int i=index; i<na-1; i++)
        allocated[i] = allocated[i+1];

    na--;

    printf("\n\tMemory for process %s DEALLOCATED !", p.id);

}

void display_mem()
{
    printf("\n\n\tALLOCATED MEMORY\n");

	if(na == 0)
		printf("\n\tNULL");

	else
		print_mem(allocated, na);

	printf("\n\n\n\n\tFREEPOOL MEMORY\n");

	if(nf == 0)
		printf("\n\tNULL");

	else
		print_mem(freepool, nf);

	printf("\n\n\n\tPHYSICAL MEMORY\n");

	if(np == 0)
		printf("\n\tNULL");

	else
		print_mem(physical, np);
}

void allocation_menu(char algo[])
{
	char choice;

	do
	{
		system("clear");

		printf("\n\n\t+----------------------------------+\n");
		printf("\t\t%s\n", algo);
		printf("\t+----------------------------------+\n\n");

		printf("\t1. ALLOCATE\n\n");
		printf("\t2. DEALLOCATE\n\n");
		printf("\t3. DISPLAY\n\n");
        printf("\t4. COALESCE HOLES\n\n");
		printf("\t5. EXIT\n\n");

		printf("\n\n\tENTER choice: ");
		scanf(" %c", &choice);

		switch(choice)
		{
			case '1':	allocate(algo);
						getch();
						break;

			case '2':   deallocate();
						getch();
						break;

			case '3':	display_mem();

						getch();
						break;

            case '4':	coalesce_holes();

                        getch();
                        break;

			case '5':	return;
						break;

			default:	printf("\n\tINVALID OPTION .....!!!!!\n\n");
						getch();
		}
	}while(choice!=4);
}


// MAIN PROGRAM

int main()
{
	char choice;

	system("clear");

	///*
	printf("\n\n\t+---------------------------------+\n");
	printf("\t\tMEMORY PARTITIONING\n");
	printf("\t+---------------------------------+\n\n");

	printf("\n\tEnter the number of partitions: ");
	scanf(" %d", &n);

	read_partitions();
	//*/

	/*
	//For debugging purposes

    original[0].start = 120;
    original[0].end = 420;
    original[0].size = 300;
    strcpy(original[0].status, "H");

    original[1].start = 420;
	original[1].end = 1020;
	original[1].size = 600;
    strcpy(original[1].status, "H");

    original[2].start = 1020;
	original[2].end = 1370;
	original[2].size = 350;
    strcpy(original[2].status, "H");

    original[3].start = 1370;
	original[3].end = 1570;
	original[3].size = 200;
    strcpy(original[3].status, "H");

    original[4].start = 1570;
	original[4].end = 2320;
	original[4].size = 750;
    strcpy(original[4].status, "H");

    original[5].start = 2320;
	original[5].end = 2445;
	original[5].size = 125;
    strcpy(original[5].status, "H");

    nf = 6;
	np = 6;
	n = 6;

	reset();
	*/
	display_mem();

	getch();


	//DISPLAYING THE MENU

	do
	{
		system("clear");

		printf("\n\n\t+----------------------------+\n");
		printf("\t\tMENU OPTIONS\n");
		printf("\t+----------------------------+\n\n");

		printf("\t1. FIRST FIT\n\n");
		printf("\t2. BEST FIT\n\n");
		printf("\t3. WORST FIT\n\n");
		printf("\t4. EXIT\n\n");

		printf("\n\n\tENTER choice: ");
		scanf(" %c", &choice);

		switch(choice)
		{
			case '1':	reset();
			            allocation_menu("FIRST FIT ALLOCATION");

						getch();
						break;

			case '2':	reset();
			            allocation_menu("BEST FIT ALLOCATION");

						getch();
						break;

			case '3':	reset();
			            allocation_menu("WORST FIT ALLOCATION");

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

