#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int picoshell(char **cmds[])
{
    int i, num=0, fd[2], prev_fd=-1;
    pid_t pid;
    while (cmds[num])
        num++;
    for (i = 0; i < num; i++)
    {
        if (i < num - 1 && pipe(fd) == -1)
            return (1);
        if ((pid = fork()) == -1)
        {
            if (i < num - 1)
            {
                close(fd[0]);
                close(fd[1]);
            }
            if (prev_fd != -1)close(prev_fd);
            return (1);
        }
        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (i < num - 1)
            {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
            }
            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }
        
        if (prev_fd != -1)
            close(prev_fd);
        if (i < num - 1)
        {
            close(fd[1]);
            prev_fd = fd[0];
        }
    }
    while (i-- > 0)
        wait(NULL);
    return (0);
}
