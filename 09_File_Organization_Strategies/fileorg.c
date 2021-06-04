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

#define M 3
#define N 25

int count = 0;

struct node
{
    char dname[100];

    char files[M][100];
    char dir[M][100];

    int nf;
    int nd;

    struct node *dir1;
    struct node *dir2;
    struct node *dir3;
};

// function to create a new tree node
struct node* newNode(char dirname[])
{
    struct node *temp = (struct node*)malloc(sizeof(struct node));

    strcpy(temp->dname, dirname);

    temp->nf = 0;
    temp->nd = 0;

    temp->dir1 = NULL;
    temp->dir2 = NULL;
    temp->dir2 = NULL;

    return temp;
}

// A structure to represent the queue
struct Queue
{
    int front, rear;
    int size;
    unsigned capacity;
    struct node **array;
};

struct Queue* createQueue()
{
    unsigned capacity = 25;

    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1; // This is important, see the enqueue
    queue->array = (struct node**) malloc(queue->capacity * sizeof(struct node*));
    return queue;
}

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{ return (queue->size == 0); }

// Queue is full when size becomes equal to the capacity
int isFull(struct Queue* queue)
{  return (queue->size == queue->capacity);  }

int size(struct Queue* queue)
{  return queue->size; }

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, struct node* item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

// Function to remove an item from queue.
// It changes front and size
struct node* dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return NULL;

    struct node* item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

struct snode
{
    char filename[N];
    int startaddr;
};

struct snode single[N];
int ws = 0;

int lower = 100;
int upper = 99999;

int duplicate_addr(int n)
{
    for(int k=0; k<ws; k++)
    {
        if(n == single[k].startaddr)
        {
            return 1;
        }
    }

    return 0;
}

int rand_startaddr()
{
    // Using current time as the seed for random generator
    srand(time(0));

    int rnum;
    rnum = (rand() % (upper - lower + 1)) + lower;

    if(duplicate_addr(rnum) == 1)
    {
        while(duplicate_addr(rnum) == 1)
        {
            rnum = (rand() % (upper - lower + 1)) + lower;
        }

    }

    return rnum;
}

void create(char fileorg[], struct node *directories[N])
{
    struct snode f;
    int flag = 0;

    if(strcmp(fileorg, "SINGLE LEVEL DIRECTORY") == 0)
    {
        printf("\n\tEnter the name of the file: ");
        scanf("%s", f.filename);

        for(int i=0; i<ws; i++)
        {
            if(strcmp(f.filename, single[i].filename) == 0)
            {
                flag = 1;
                break;
            }
        }

        if(flag == 0)
        {
            f.startaddr = rand_startaddr();

            single[ws++] = f;
            printf("\n\n\t"
                   "%s was CREATED at STARTING LOCATION %d !", f.filename, f.startaddr);
        }

        else
        {
            printf("\n\n\tFilename %s ALREADY EXISTS !\n\tTry a DIFFERENT FILENAME !", f.filename);
        }
    }

    else if (strcmp(fileorg, "TREE STRUCTURE DIRECTORY") == 0)
    {
        char choice;
        char fname[N], dname[N];
        char pdname[N];

        printf("\n\tCreate a FILE or DIRECTORY?[f/d]: ");
        scanf(" %c", &choice);

        switch(choice)
        {
            case 'f':   printf("\n\tEnter the name of the file: ");
                        scanf("%s", fname);

                        printf("\n\n\tThe TOP MOST directory is called \"root\".\n");

                        printf("\n\tEnter the name of the parent directory: ");
                        scanf("%s", pdname);

                        for(int i=0; i<count; i++)
                        {
                            if(strcmp(directories[i]->dname, pdname) == 0)
                            {
                                if(directories[i]->nf == M)
                                {
                                    printf("\n\tMax number of files reached !\n\tCannot create %s in %s !", fname, directories[i]->dname);
                                    return;
                                }

                                else
                                {
                                    for(int h=0; h<directories[i]->nf; h++)
                                    {
                                        if(strcmp(directories[i]->files[h], fname) == 0)
                                        {
                                            printf("\n\n\tCANNOT CREATE: file with name %s already exists in %s!", fname, pdname);
                                            return;
                                        }
                                    }

                                    flag = 1;
                                    strcpy(directories[i]->files[directories[i]->nf++], fname);
                                    break;
                                }
                            }
                        }

                        if(flag == 0)
                            printf("\n\tINVALID parent directory name !");

                        else
                            printf("\n\n\tFILE %s CREATED in %s!", fname, pdname);

                        break;

            case 'd':   printf("\n\tEnter the name of the directory: ");
                        scanf("%s", dname);

                        struct node *temp = newNode(dname);

                        printf("\n\n\tThe TOP MOST directory is called \"root\".\n");

                        printf("\n\tEnter the name of the parent directory: ");
                        scanf("%s", pdname);

                        for(int i=0; i<count; i++)
                        {
                            if(strcmp(directories[i]->dname, pdname) == 0)
                            {
                                if(directories[i]->nd == M)
                                {
                                    printf("\n\tMax number of sub-directories reached !\n\tCannot create %s in %s !", dname, directories[i]->dname);
                                    return;
                                }

                                else
                                {
                                    for(int h=0; h<directories[i]->nd; h++)
                                    {
                                        if(strcmp(directories[i]->dir[h], dname) == 0)
                                        {
                                            printf("\n\n\tCANNOT CREATE: directory with name %s already exists in %s!", dname, pdname);
                                            return;
                                        }
                                    }


                                    if(directories[i]->nd == 0)
                                    {
                                        directories[i]->dir1 = temp;
                                    }

                                    else if (directories[i]->nd == 1)
                                    {
                                        directories[i]->dir2 = temp;
                                    }

                                    else if (directories[i]->nd == 2)
                                    {
                                        directories[i]->dir3 = temp;
                                    }

                                    flag = 1;
                                    strcpy(directories[i]->dir[directories[i]->nd++], dname);
                                    break;
                                }
                            }
                        }

                        if(flag == 0)
                            printf("\n\tINVALID parent directory name !");

                        else
                        {
                            printf("\n\n\tDIRECTORY %s CREATED in %s!", dname, pdname);
                            directories[count++] = temp;
                        }

                        break;

            default :   printf("\n\tINVALID choice !");
                        return;
        }

    }

}

void display(char fileorg[], struct node *directories[N])
{
    if(strcmp(fileorg, "SINGLE LEVEL DIRECTORY") == 0)
    {
        printf("\n\t\tSINGLE LEVEL DIRECTORY");
        printf("\n\n\t-----------------------------");
        printf("\n\tFILENAME\tSTARTING ADDRESS");
        printf("\n\t-----------------------------");

       for(int i=0; i<ws; i++)
       {
           printf("\n\t%-8s\t%16d", single[i].filename, single[i].startaddr);
       }
    }

    else if (strcmp(fileorg, "TREE STRUCTURE DIRECTORY") == 0)
    {
        printf("\n\t\tTREE STRUCTURE DIRECTORY\n");

        for(int d=0; d<count; d++)
        {
            printf("\n\n\tDirectory: %s", directories[d]->dname);
            printf("\n\tFiles: ");

            if(directories[d]->nf == 0)
                printf("None");

            for(int f=0; f<directories[d]->nf; f++)
                printf("%s ",directories[d]->files[f]);

            printf("\n\tSub-directories: ");

            if(directories[d]->nd == 0)
                printf("None");

            for(int h=0; h<directories[d]->nd; h++)
            {
                printf("%s ",directories[d]->dir[h]);
            }
        }

        printf("\n\n\tLEVEL ORDER TRAVERSAL is displayed");

        struct Queue *q = createQueue();

        enqueue(q, directories[0]); // Enqueue root
        printf("\n\n\t");
        while (!isEmpty(q))
        {
            int n = size(q);

            // If this node has children
            while (n > 0)
            {
                // Dequeue an item from queue and print it
                struct node *p = dequeue(q);
                printf("%s", p->dname);

                for(int i=0; i<p->nf; i++)
                {
                    if(i == 0)
                        printf("(");

                    printf("%s ", p->files[i]);

                    if(i == p->nf - 1)
                        printf("\b)");
                }

                printf("\t");

                // Enqueue all children of the dequeued item
                if (p->nd == 1)
                    enqueue(q, p->dir1);

                else if (p->nd == 2) {
                    enqueue(q, p->dir1);
                    enqueue(q, p->dir2);
                } else if (p->nd == 3) {
                    enqueue(q, p->dir1);
                    enqueue(q, p->dir2);
                    enqueue(q, p->dir3);
                }

                n--;
            }

            printf("\n\n\t"); // Print new line between two levels
        }
    }

}

void file_menu(char fileorg[])
{
    char choice;

    struct node *root = newNode("root");
    struct node *directories[N];

    directories[count] = (struct node*)malloc(sizeof(struct node));
    directories[count++] = root;

    do
    {
        system("clear");

        printf("\n\n\t+----------------------------------+\n");
        printf("\t\t%s\n", fileorg);
        printf("\t+----------------------------------+\n\n");

        printf("\t1. CREATE A FILE\n\n");
        printf("\t2. DISPLAY ALL FILES\n\n");
        printf("\t3. EXIT\n\n");

        printf("\n\n\tENTER choice: ");
        scanf(" %c", &choice);

        switch(choice)
        {
            case '1':	create(fileorg, directories);
                        getch();
                        break;

            case '2':   display(fileorg, directories);
                        getch();
                        break;

            case '3':	return;
                        break;

            default:	printf("\n\tINVALID OPTION .....!!!!!\n\n");
                        getch();
        }
    }while(choice != 3);
}

// MAIN PROGRAM

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

        printf("\t1. SINGLE LEVEL DIRECTORY\n\n");
        printf("\t2. TREE STRUCTURE DIRECTORY\n\n");
        printf("\t3. EXIT\n\n");

        printf("\n\n\tENTER choice: ");
        scanf(" %c", &choice);

        switch(choice)
        {
            case '1':	file_menu("SINGLE LEVEL DIRECTORY");
                        break;

            case '2':	file_menu("TREE STRUCTURE DIRECTORY");
                        break;

            case '3':	system("clear");
                        return 0;
                        break;

            default:	printf("\n\tINVALID OPTION .....!!!!!\n\n");
                        getch();
        }
    }while(choice != 3);
}