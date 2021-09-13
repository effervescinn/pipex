#include "pipex.h"

void	second_fd_error(int fd_out, t_info *info)
{
	if (fd_out < 0)
	{
		if (errno == 13 || errno == 9)
		{
			write(2, "zsh: permission denied: ", 25);
			write(2, info->file2, ft_strlen(info->file2));
			write(2, "\n", 1);
		}
		exit(0);
	}
}

void	second_cmd(t_info *info, char **env)
{
	int		fd_out;
	char	*cmd;

	cmd = find_bin(env, info->cmd2[0]);
	info->pids[1] = fork();
	if (info->pids[1] == 0)
	{
		if (!cmd)
		{
			write(2, "zsh: command not found: ", 25);
			write(2, info->cmd2[0], ft_strlen(info->cmd2[0]));
			write(2, "\n", 1);
			exit(0);
		}
		dup2(info->fd[0], STDIN_FILENO);
		fd_out = open(info->file2, O_CREAT | O_WRONLY | O_TRUNC, 0777);
		second_fd_error(fd_out, info);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
		close(info->fd[0]);
		close(info->fd[1]);
		execve(cmd, info->cmd2, 0);
	}
	free(cmd);
}

void	first_fd_error(int fd_in, t_info *info)
{
	if (fd_in < 0)
	{
		if (errno == 2)
		{
			write(2, "zsh: no such file or directory: ", 33);
			write(2, info->file1, ft_strlen(info->file1));
			write(2, "\n", 1);
		}
		else if (errno == 13 || errno == 9)
		{
			write(2, "zsh: permission denied: ", 25);
			write(2, info->file1, ft_strlen(info->file1));
			write(2, "\n", 1);
		}
		exit(0);
	}
}

void	first_cmd(t_info *info, char **env)
{
	int		fd_in;
	char	*cmd;

	cmd = find_bin(env, info->cmd1[0]);
	info->pids[0] = fork();
	if (info->pids[0] == 0)
	{
		dup2(info->fd[1], STDOUT_FILENO);
		fd_in = open(info->file1, O_RDONLY);
		first_fd_error(fd_in, info);
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
		close(info->fd[0]);
		close(info->fd[1]);
		if (!cmd)
		{
			write(2, "zsh: command not found: ", 25);
			write(2, info->cmd1[0], ft_strlen(info->cmd1[0]));
			write(2, "\n", 1);
			exit(0);
		}
		execve(cmd, info->cmd1, 0);
	}
	free(cmd);
}
