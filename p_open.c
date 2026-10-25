#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
	if(!file || !argv || (type!= 'r' && type!='w'))
        return -1;
	int fd [2];
	pipe(fd);
	int i = fork();
	if (i == 0)
	{
		if (type == 'r') 
            dup2(fd[1],1);
		else
            dup2(fd[0],0);
		close (fd[0]);
		close (fd[1]);
		execvp(file ,argv);
	}
	if (type == 'r')
	{
		close(fd[1]);
		return fd[0];
	}
	else
	{
		close(fd[0]);
		return fd[1];
	}
}