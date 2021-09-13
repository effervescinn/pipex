#ifndef PIPEX_H
# define PIPEX_H
# include "stdio.h"
# include "unistd.h"
# include "stdlib.h"
# include <fcntl.h>
# include <sys/errno.h>
# include "libft/libft.h"

typedef struct s_info
{
	char	*file1;
	char	**cmd1;
	char	**cmd2;
	char	*file2;
	int		pids[2];
	int		fd[2];
}				t_info;

int		parser(int ac, char **av, t_info *info);
void	split_env(char **env, char ***pths);
char	*find_bin(char **env, char *pipex_cmd);
void	free_info(t_info *info);
void	first_cmd(t_info *info, char **env);
void	first_fd_error(int fd_in, t_info *info);
void	second_cmd(t_info *info, char **env);
void	second_fd_error(int fd_out, t_info *info);

#endif