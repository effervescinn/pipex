#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include <fcntl.h>
#include <sys/errno.h>
#include "libft/libft.h"

typedef struct s_info
{
	char *file1;
	char **cmd1;
	char **cmd2;
	char *file2;
	int pids[2];
	int fd[2];
} t_info;

int parser(int ac, char **av, t_info *info)
{
	int i;
	char *cmd1;
	char *cmd2;

	if (ac != 5)
		return (1);
	i = 0;
	info->file1 = ft_strdup(av[1]);
	cmd1 = ft_strdup(av[2]);
	cmd2 = ft_strdup(av[3]);
	info->file2 = ft_strdup(av[4]);
	info->cmd1 = ft_split(cmd1, ' ');
	info->cmd2 = ft_split(cmd2, ' ');
	free(cmd1);
	free(cmd2);
	return (0);
}

char *find_bin(char **env, char *pipex_cmd)
{
	char **pths;
	char *tmp;
	char *cmd_path;
	char *cmd;
	int i;

	while (*env)
	{
		if (!(ft_strncmp(*env, "PATH=", 5)))
		{
			pths = ft_split(*env, ':');
			break;
		}
		(*env)++;
	}
	i = -1;
	while (pths[++i])
	{
		tmp = ft_strjoin(pths[i], "/");
		cmd = ft_strjoin(tmp, pipex_cmd);
		free(tmp);
		if (!(access(cmd, 1)))
		{
			i = -1;
			while (pths[++i])
				free(pths[i]);
			free(pths);
			return (cmd);
		}
	}
	return (NULL);
}

int main(int ac, char **av, char **env)
{
	t_info info;
	char *cmd;
	int fd_out;
	int fd_in;

	if (parser(ac, av, &info))
		return (1);
	pipe(info.fd);

	cmd = find_bin(env, info.cmd1[0]);
	info.pids[0] = fork();
	if (info.pids[0] == 0)
	{
		dup2(info.fd[1], STDOUT_FILENO);
		fd_in = open(info.file1, O_RDONLY);
		if (fd_in < 0)
		{
			if (errno == 2)
			{
				write(2, "no such file or directory: ", 28);
				write(2, info.file1, ft_strlen(info.file1));
				write(2, "\n", 1);
			}
			else if (errno == 13 || errno == 9)
				write(2, "permission denied\n", 21);
			exit(0);
		}
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
		close(info.fd[0]);
		close(info.fd[1]);
		execve(cmd, info.cmd1, 0);
	}

	cmd = find_bin(env, info.cmd2[0]);
	info.pids[1] = fork();
	if (info.pids[1] == 0)
	{
		dup2(info.fd[0], STDIN_FILENO);
		fd_out = open(info.file2, O_CREAT | O_WRONLY | O_TRUNC, 0777);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
		close(info.fd[0]);
		close(info.fd[1]);
		execve(cmd, info.cmd2, 0);
	}
	close(info.fd[0]);
	close(info.fd[1]);
	waitpid(info.pids[0], NULL, 0);
	waitpid(info.pids[1], NULL, 0);

	return (0);
}
