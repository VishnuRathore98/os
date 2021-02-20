#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;

	//Fork a child process.
	pid = fork();
	
	if (pid< 0)
	{
		fprintf(stderr,"Fork Failed");
		return 1;
	}
	else if (pid == 0)
	{
		execlp("/bin/ls", "ls", NULL);	//Child Process
	}
	else
	{	//Parent Process
		//Parent will wait for the child to complete
		wait(NULL);
		printf("\nChild Complete\n");

	}
	return 0;
}

