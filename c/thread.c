#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

int sum;
void *runner(void *param);

int main(int argc, char *argv[])
{
	pthread_t tid;
	pthread_attr_t attr;

	if(argc !=2)
	{
		fprintf(stderr, "Usage: a.out <integer value>\n");
		return -1;
	}
	if(atoi(argv[1]) < 0)
	{	
		fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
		return -1;	
	}

	pthread_attr_init(&attr);
	printf("Creating thread...\n");
	pthread_create(&tid, &attr, runner, argv[1]);
	pthread_join(tid, NULL);
	printf("Exiting thread\n");
	printf("sum = %d\n", sum);

}

void *runner(void *param)
{
	int i, upper = atoi(param);
	sum = 0;

	for(i=1; i<=upper; i++)
	sum += 1;

	pthread_exit(0);
}
