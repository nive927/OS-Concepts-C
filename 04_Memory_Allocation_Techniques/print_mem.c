#include<stdio.h>
#include <string.h>
#include <stdlib.h>

void print_mem1(struct mnode arr[], int ws)
{
	int i, j;

	// printing the TOP BAR
	printf("\n\n\t ");
	for(i=0; i<ws; i++)
	{
		for(j=0; j<arr[i].size; j++)
			printf("-");
		printf(" ");
	}

	// printing the STATUS in the middle
	printf("\n\t|");
	for(i=0; i<ws; i++)
	{
		for(j=0; j<((arr[i].size)-strlen(arr[i].status)); j++)
			printf(" ");
		printf("%s", arr[i].status);

		printf("|");
	}

	printf("\n\t ");

	// printing the bottom bar
	for(i=0; i<ws; i++)
	{
		for(j=0; j<arr[i].size; j++)
			printf("-");
		printf(" ");
	}

	printf("\n\t");

	// printing the bytes line
	for(i=0; i<ws; i++)
	{
		printf("%d", arr[i].start);
		for(j=0; j<arr[i].size; j++)
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

// Fixed width
void print_mem2(struct mnode arr[], int ws)
{
	int i, j;
	int node_width = 10;

	// printing the TOP BAR
	printf("\n\n\t ");
	for(i=0; i<ws; i++)
	{
		for(j=0; j<node_width; j++)
			printf("-");
		printf(" ");
	}

	// printing the STATUS in the middle
	printf("\n\t|");
	for(i=0; i<ws; i++)
	{
		for(j=0; j<((node_width)-strlen(arr[i].status)); j++)
			printf(" ");
		printf("%s", arr[i].status);

		printf("|");
	}

	printf("\n\t ");

	// printing the bottom bar
	for(i=0; i<ws; i++)
	{
		for(j=0; j<node_width; j++)
			printf("-");
		printf(" ");
	}

	printf("\n\t");

	// printing the bytes line
	for(i=0; i<ws; i++)
	{
		printf("%d", arr[i].start);
		for(j=0; j<node_width; j++)
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

// The final in-use function
// varies based on size like required

void print_mem3(struct mnode arr[], int ws)
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

	newMin = 8;
	newMax = 18;

	newRange = newMax - newMin;

	for(i=0; i<ws; i++)
		node_width[i] = ((arr[i].size - oldMin) * newRange / oldRange) + newMin;

	for(i = 0; i<ws; i++)
		printf("\n\tnode_width %d: %d", (i+1), node_width[i]);


	// printing the TOP BAR
	printf("\n\n\t ");
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
		for(j=0; j<((node_width[i])-strlen(arr[i].status)); j++)
			printf(" ");
		printf("%s", arr[i].status);

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
