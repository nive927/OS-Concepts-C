#include<stdio.h>
#include<stdlib.h>

typedef struct
{
	char pid[10];
	int arr;
	int burst;
	int pri;
	int rem;
	int wait;
	int turn;
	int flag;
}job;

void rr(job ar[],int n)
{
	int time=0;
	float avgturn=0,avgwait=0;
	int min=9999;
	int index;
	job temp;
	
	for(int i=0;i<n-1;i++)
	{
		for(int j=i+1;j<n;j++)
		{
			if(ar[j].arr<ar[i].arr)
			{
				temp=ar[j];
				ar[j]=ar[i];
				ar[i]=temp;
			}
		
		}
	
	}
	
	for(int i=0;i<n;i++)
	{
		time+=ar[i].burst;
	}
	
	printf("\n");
	int i=0;
	for(int t=0;t<time;)
	{
		if(i<n)
		{
			ar[i].wait=t;
		
			t+=ar[i].burst;
		
			ar[i].turn=t;
		
			i++;
		}
		else
			break;
	}
	printf("\nProcess ID\tArrival Time\tBurst Time\tTurnaround\tWaiting Time\n");
	for(int i=0;i<n;i++)
	{
		printf("\n%s\t\t%d\t\t%d\t\t%d\t\t%d",ar[i].pid,ar[i].arr,ar[i].burst,ar[i].turn,ar[i].wait);
		avgturn+=ar[i].turn;
		avgwait+=ar[i].wait;
	}
	printf("\n Average \t\t\t\t\t%.2f\t\t%.2f",avgturn/n,avgwait/n);
	printf("\n");
}

void priority_np(job ar[],int n)
{
	int time=0;
	float avgturn=0,avgwait=0;
	int min;
	int index;
	
	printf("\n\n\t\tPRIORITY (Non Pre Emptive)\n");
	time=0;
	for(int i=0;i<n;i++)
	{
		time+=ar[i].burst;
	}
	
	printf("\nProcess ID\tArrival Time\tBurst Time\tTurnaround\tWaiting Time\n");
	for(int t=0;t<time;)
	{
		min=9999;
		for(int i=0;i<n;i++)
		{
			if(ar[i].arr<=t && ar[i].pri<min && ar[i].flag==0)
			{
				min=ar[i].pri;
				index=i;
			}
		}
		ar[index].flag=1;
		ar[index].wait=t-ar[index].arr;
		t+=ar[index].burst;
		ar[index].turn=t-ar[index].arr;
		printf("\n%s\t\t%d\t\t%d\t\t%d\t\t%d",ar[index].pid,ar[index].arr,ar[index].burst,ar[index].turn,ar[index].wait);
		avgturn+=ar[index].turn;
		avgwait+=ar[index].wait;
	}
	
	printf("\n Average \t\t\t\t\t%.2f\t\t%.2f",avgturn/n,avgwait/n);
	printf("\n");
}

void priority_p(job ar[],int n)
{
	int time=0;
	float avgturn=0,avgwait=0;
	int min;
	int index;
	int prev=-1;
	printf("\n\n\t\tPRIORITY (Pre Emptive)\n");
	time=0;
	int remain=0,endtime;
	
	for(int i=0;i<n;i++)
	{
		time+=ar[i].burst;
	}
	
	printf("\nProcess ID\tArrival Time\tBurst Time\tTurnaround\tWaiting Time\n");
	for(int t=0;remain!=n;t++)
	{
		min=9999;
		for(int i=0;i<n;i++)
		{
			if( ar[i].arr<=t && ar[i].pri<min && ar[i].rem>0)
			{
				min=ar[i].rem;
				index=i;
			}
		}
		
		ar[index].rem-=1;
		
		if(ar[index].rem==0)
		{
			remain++;
			endtime=t+1;
			ar[index].turn=endtime-ar[index].arr;
			ar[index].wait=endtime-ar[index].burst-ar[index].arr;
		}	
	}
	for(int i=0;i<n;i++)
	{
		printf("\n%s\t\t%d\t\t%d\t\t%d\t\t%d",ar[i].pid,ar[i].arr,ar[i].burst,ar[i].turn,ar[i].wait);
		avgturn+=ar[i].turn;
		avgwait+=ar[i].wait;
	}
	printf("\n Average \t\t\t\t\t%.2f\t\t%.2f",avgturn/n,avgwait/n);
	printf("\n");
}

void input(job ar[],int n)
{
	for(int i=0;i<n;i++)
	{
		printf("\nEnter PID : ");
		scanf("%s",ar[i].pid);
		printf("Enter Arrival Time : ");
		scanf("%d",&ar[i].arr);
		printf("Enter Burst Time : ");
		scanf("%d",&ar[i].burst);
		printf("Enter Priority : ");
		scanf("%d",&ar[i].pri);
		ar[i].rem=ar[i].burst;
		ar[i].flag=0;
	}
	printf("\n");
}

int main()
{
	job ar[10];
	int n;
	int time=0;
	float avgturn=0,avgwait=0;
	int min;
	int index;
	int choice=3;
	while(choice!=0)
	{
		printf("\n\n\t\tCPU SCHEDULING ALGORITHMS\n");
		printf("1.ROUND ROBIN\n2.PRIORITY\n0.EXIT\nEnter Choice : ");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:			
				printf("\t\tROUND ROBIN CPU SCHEDULER\n");
				printf("Enter Number of Processes : ");
				scanf("%d",&n);
				input(ar,n);
				rr(ar,n);
				break;
			case 2:
				printf("\t\tPRIORITY CPU SCHEDULER\n");
				printf("1. Non Preemptive PRIORITY\n2. Pre emptive PRIORITY\nEnter your option : ");
				scanf("%d",&choice);
				printf("Enter Number of Processes : ");
				scanf("%d",&n);
				input(ar,n);
				if(choice==1)
					priority_np(ar,n);
				else if(choice==2)
					priority_p(ar,n);
				else
					printf("Invalid Choice !!!\n");
				break;
		}
	}
	return 0;
}
