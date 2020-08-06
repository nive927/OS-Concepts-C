// Need to link the pthread library for compilation
// gcc -Wall statistics.c -lpthread -o run

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int n;
float average, minimum, maximum;

void *th1(char *argv[])
{
	float sum = 0;

	for(int i=1; i<n; i++)
		sum += atof(argv[i]);

	average = sum/(n-1);

	pthread_exit(0);
}
void *th2(char *argv[])
{
	minimum = atof(argv[1]);

	for(int i=1; i<n; i++)
		if(minimum > atof(argv[i]))
			minimum = atof(argv[i]);

	pthread_exit(0);
}
void *th3(char *argv[])
{
	maximum = atof(argv[1]);

	for(int i=1; i<n; i++)
		if(maximum < atof(argv[i]))
			maximum = atof(argv[i]);

	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr,"\nINSUFFICIENT number of ARGUMENTS !\n");
		return -1;
	}

	// Thread identifiers

	pthread_t t1;
	pthread_t t2;
	pthread_t t3;

	// Set of thread attributes
	pthread_attr_t attr;

	// Getting the defailt thread attributes
	pthread_attr_init(&attr);

	n = argc;

	pthread_create(&t1, &attr, &th1, argv);
	pthread_join(t1,NULL);
	printf("\nThe average value is %f", average);

	pthread_create(&t2, &attr, &th2, argv);
	pthread_join(t2,NULL);
	printf("\nThe minimum value is %f", minimum);

	pthread_create(&t3, &attr, &th3, argv);
	pthread_join(t3,NULL);
	printf("\nThe maximum value is %f\n", maximum);

	return 0;
}