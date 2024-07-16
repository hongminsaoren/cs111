#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int err;
	// TODO: it's all yours
	if (argc == 1)
	{
		perror("No arguments");
		exit(EINVAL);
	}
	if (argc == 2)
	{
		err = execlp(argv[1], argv[1], NULL);
		if (err == -1)
		{
			perror("execlp");
			exit(EINVAL);
		}
	}
	else
	{
		int i;
		for (i = 1; i < argc - 1; ++i)
		{
			int pipe_fd[2];
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			pid_t cpid;
			cpid = fork();
			if (cpid == -1)
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}
			if (cpid == 0)
			{
				dup2(pipe_fd[1], 1);
				close(pipe_fd[0]);
				close(pipe_fd[1]);
				err = execlp(argv[i], argv[i], NULL);
				printf("err value %d\n", err);
				if (err == -1)
				{
					fprintf(stderr, "execlp fails in executable %d\n", i);

					exit(EINVAL);
				}
				exit(0);
			}
			int stat;
			waitpid(cpid, &stat, 0);
			// printf("stat value %d\n", stat);
			// printf("Child exited with status %d\n", WEXITSTATUS(stat));
			if (WEXITSTATUS(stat) > 0)
			{
				exit(WEXITSTATUS(stat));
			}
			dup2(pipe_fd[0], 0);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}

		pid_t cpid_last;
		cpid_last = fork();
		if (cpid_last == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (cpid_last == 0)
		{
			err = execlp(argv[i], argv[i], NULL);
			if (err == -1)
			{
				perror("execlp fails in the last executable");
				exit(EINVAL);
			}
			exit(0);
		}
		int stat;
		waitpid(cpid_last, &stat, 0);
		if (WEXITSTATUS(stat) > 0)
		{
			exit(WEXITSTATUS(stat));
		}
	}
	return 0;
}