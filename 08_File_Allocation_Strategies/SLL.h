#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 25

struct file
{
	char name[N];
	int size;
	int numBlocks;
};

struct mnode
{
	int bid;
	struct file f;
	int fileBlockTable[N];
};

struct node
{
  struct mnode data;
  struct node *next;
  struct node *fnext;
};

void append(struct node** head, int id)
{
    struct node* newNode = (struct node*) malloc(sizeof(struct node));

    struct node *last = *head;

    newNode->data.bid  = id;
    strcpy(newNode->data.f.name, "free");
    newNode->fnext  = NULL;
    newNode->next = NULL;

    if (*head == NULL)
    {
       *head = newNode;
       return;
    }

    while (last->next != NULL)
        last = last->next;

    last->next = newNode;
}

void display(struct node** head)
{
    struct node* current = *head;

    while (current != NULL)
    {
        printf("\n\tBlock ID: %d", current->data.bid);
        current = current->next;
    }
}