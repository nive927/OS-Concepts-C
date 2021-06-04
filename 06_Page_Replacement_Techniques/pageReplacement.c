
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

#define INF 9999
int i, j, k;

int n, nf;
int pgRefStr[100], page[50];

int hit = 0;
int pgfault = 0;

void read_data()
{
    printf("\n\tEnter page reference string length: ");
    scanf("%d",&n);

    printf("\n\tEnter the page reference string: ");
    for(i=0; i<n; i++)
        scanf("%d",&pgRefStr[i]);

    printf("\n\tEnter the number of frames: ");
    scanf(" %d",&nf);

}

void initialize()
{
    pgfault = 0;
    for(i=0; i<nf; i++)
        page[i] = INF;
}

int isHit(int data)
{
    hit = 0;
    for(j=0; j<nf; j++)
    {
        if(page[j] == data)
        {
            hit=1;
            break;
        }
    }

    return hit;
}

int getHitIndex(int data)
{
    int hitind;
    for(k=0; k<nf; k++)
    {
        if(page[k] == data)
        {
            hitind = k;
            break;
        }
    }

    return hitind;
}

void dispPages()
{
    for (k=0; k<nf; k++)
    {
        if(page[k] != INF)
            printf(" %d", page[k]);
    }
}

void fifo()
{
    initialize();

    for(i=0; i<n; i++)
    {
        printf("\n\t %d:",pgRefStr[i]);

        if(isHit(pgRefStr[i]) == 0)
        {
            for(k=0; k<nf-1; k++)
                page[k] = page[k+1];

            page[k] = pgRefStr[i];

            pgfault++;

            dispPages();
        }

    	else
            printf(" NO page fault !");
    }

    printf("\n\n\tTotal number of PAGE FAULTS: %d", pgfault);
}


void optimal()
{
    initialize();

    int near[50];

    for(i=0; i<n; i++)
    {
        printf("\n\t %d:",pgRefStr[i]);

        if(isHit(pgRefStr[i]) == 0)
        {
            for(j=0; j<nf; j++)
            {
                int pg = page[j];
                int found = 0;

                for(k=i; k<n; k++)
                {
                    if(pg == pgRefStr[k])
                    {
                        near[j] = k;
                        found = 1;
                        break;
                    }

                    else
                        found = 0;
                }

                if(!found)
                    near[j] = INF;
            }

 			int max = -INF;
            int repindex;

            for(j=0; j<nf; j++)
            {
                if(near[j] > max)
                {
                    max = near[j];
                    repindex = j;
                }
            }

            page[repindex] = pgRefStr[i];
            pgfault++;

            dispPages();
        }

        else
             printf(" NO page fault !");
    }

    printf("\n\n\tTotal number of PAGE FAULTS: %d", pgfault);
}

void lru()
{
    initialize();

    int least[50];

    for(i=0; i<n; i++)
    {

        printf("\n\t %d :",pgRefStr[i]);

        if(isHit(pgRefStr[i]) == 0)
        {

            for(j=0; j<nf; j++)
            {
                int pg = page[j];
                int found = 0;

                for(k=i-1; k>=0; k--)
                {
                    if(pg == pgRefStr[k])
                    {
                        least[j] = k;
                        found = 1;
                        break;
                    }

                    else
                        found = 0;
                }

                if(!found)
                    least[j] = -INF;
            }

            int min = INF;
            int repindex;

            for(j=0; j<nf; j++)
            {
                if(least[j] < min)
                {
                    min = least[j];
                    repindex = j;
                }
            }

            page[repindex] = pgRefStr[i];
            pgfault++;

            dispPages();
        }

        else
             printf(" NO page fault !");
    }

    printf("\n\n\tTotal number of PAGE FAULTS: %d", pgfault);
}

void lfu()
{
    int usedcnt[100];
    int least,repin,sofarcnt=0,bn;

    initialize();

    for(i=0; i<nf; i++)
        usedcnt[i] = 0;

    for(i=0; i<n; i++)
    {
        printf("\n\t %d :" ,pgRefStr[i]);

        if(isHit(pgRefStr[i]))
        {
            int hitind = getHitIndex(pgRefStr[i]);

            usedcnt[hitind]++;
            printf(" NO page fault !");
        }

        else
        {
            pgfault++;

            if(bn < nf)
            {
                page[bn] = pgRefStr[i];
                usedcnt[bn] = usedcnt[bn]+1;
                bn++;
            }

            else
            {
                least = INF;
                for(k=0; k<nf; k++)
                    if(usedcnt[k] < least)
                    {
                        least = usedcnt[k];
                        repin = k;
                    }

                page[repin] = pgRefStr[i];
                sofarcnt = 0;

                for(k=0; k<=i; k++)
                    if(pgRefStr[i] == pgRefStr[k])
                        sofarcnt = sofarcnt+1;

                usedcnt[repin] = sofarcnt;
            }

            dispPages();
        }

    }

    printf("\n\n\tTotal number of PAGE FAULTS: %d", pgfault);
}

int main()
{
	char choice;

	system("clear");

	//DISPLAYING THE MENU

	do
	{
		system("clear");

		printf("\n\n\t+----------------------------+\n");
		printf("\t\tMENU OPTIONS\n");
		printf("\t+----------------------------+\n\n");

		printf("\t1. READ DATA\n\n");
		printf("\t2. FIFO\n\n");
		printf("\t3. OPTIMAL\n\n");
		printf("\t4. LRU\n\n");
		printf("\t5. LFU\n\n");
		printf("\t6. EXIT\n\n");

		printf("\n\n\tENTER choice: ");
		scanf(" %c", &choice);

		switch(choice)
		{
			case '1':	read_data();

						getch();
						break;

			case '2':	fifo();

						getch();
						break;

			case '3':	optimal();

						getch();
						break;

			case '4':	lru();

						getch();
						break;

			case '5':	lfu();

						getch();
						break;

			case '6':	system("clear");
						return 0;
						break;

			default:	printf("\n\tINVALID OPTION .....!!!!!\n\n");
						getch();
		}
	}while(choice!=6);
}