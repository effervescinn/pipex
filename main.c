#include "pipex.h"

int	parser(int ac, char **av, t_info *info)
{
	int		i;
	char	*cmd1;
	char	*cmd2;

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

void	split_env(char **env, char ***pths)
{
	while (*env)
	{
		if (!(ft_strncmp(*env, "PATH=", 5)))
		{
			*pths = ft_split(*env, ':');
			break ;
		}
		(*env)++;
	}
}

char	*find_bin(char **env, char *pipex_cmd)
{
	char	**pths;
	char	*tmp;
	char	*cmd;
	int		i;

	split_env(env, &pths);
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
		free(cmd);
	}
	return (NULL);
}

void	free_info(t_info *info)
{
	int	i;

	i = -1;
	while (info->cmd1[++i])
		free(info->cmd1[i]);
	free(info->cmd1);
	i = -1;
	while (info->cmd2[++i])
		free(info->cmd2[i]);
	free(info->cmd2);
	free(info->file1);
	free(info->file2);
}

int	main(int ac, char **av, char **env)
{
	t_info	info;

	if (parser(ac, av, &info))
	{
		write(1, "Error: wrong arguments\n", 24);
		return (1);
	}
	if (pipe(info.fd) < 0)
	{
		write(1, "Error: pipe error\n", 11);
		free_info(&info);
		return (1);
	}
	first_cmd(&info, env);
	second_cmd(&info, env);
	close(info.fd[0]);
	close(info.fd[1]);
	waitpid(info.pids[0], NULL, 0);
	waitpid(info.pids[1], NULL, 0);
	free_info(&info);
	return (0);
}
