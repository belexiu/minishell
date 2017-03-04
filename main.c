#include <stdio.h>
#include <string.h>
#include <unistd.h>




#include <stdlib.h>
#include <sys/stat.h>

#include "ft_strsplit.h"
#include "ft_gnl.h"
#include "minishell.h"

#define PRINT_ERR 1
#define DONT_PRINT_ERR 0

int 	ft_chr_in_str(char c, char *str)
{
	while (str && *str)
	{
		if (c == *str)
			return (1);
		str++;
	}
	return (0);
}

int		ft_strcmp(char *s1, char *s2)
{
	if (s1 == NULL || s2 == NULL)
		return (-1);
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int 	ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

void	ft_print_prompt(void)
{
	write(1, "%>", 2);
}

int 	ft_verify_path(char *path, char mode, int print_error)
{
	struct stat	file_status;
	int 		exists;
	int 		can_execute;
	int 		is_reg_file;
	int 		is_dir;

	stat(path, &file_status);
	exists = access(path, F_OK) == 0;
	can_execute = access(path, X_OK) == 0;
	is_reg_file = (file_status.st_mode & S_IFMT) == S_IFREG;
	is_dir = (file_status.st_mode & S_IFMT) == S_IFDIR;
	if (print_error)
	{
		if (exists == 0)
			printf("%s: does not exist\n", path);
		else if (can_execute == 0)
			printf("%s: permission denied\n", path);
		else if (mode == 'e' && is_reg_file == 0)
			printf("%s: not an executable\n", path);
		else if (mode == 'd' && is_dir == 0)
			printf("%s: not a directory", path);
	}
	return (exists && can_execute &&
			((is_reg_file && mode == 'e') || (is_dir && mode == 'd')));
}

void	ft_fork_and_wait(char **cmd, char **envp)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid > 0)
		wait(&status);
	else if (pid == 0)
	{
		execve(cmd[0], cmd, envp);
		printf("execve: could not load executable at %s\n", cmd[0]);
		exit(1);
	}
	else
	{
		printf("fork: could not fork\n");
	}
}

char	**ft_get_split_sys_paths(char **envp)
{
	char	**line;
	char	**paths;

	paths = NULL;
	while (*envp)
	{
		line = ft_strsplit(*envp, '=');
		if (ft_strcmp(line[0], "PATH") == 0)
		{
			paths = ft_strsplit(line[1], ':');
			ft_splitfree(line);
			break;
		}
		ft_splitfree(line);
		envp++;
	}
	return (paths);
}


char	*ft_path_join(char *path, char *name)
{
	int		i;
	char	*result;

	result = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(name) + 2));
	i = 0;
	while (path && *path)
	{
		result[i] = *path;
		i++;
		path++;
	}
	result[i] = '/';
	i++;
	while (name && *name)
	{
		result[i] = *name;
		i++;
		name++;
	}
	result[i] = '\0';
	return (result);
}

void	ft_try_sys_path(char **cmd, char **envp)
{
	char	**sys_paths;
	char	*complete_path;
	int		cmd_found;
	int 	i;

	cmd_found = 0;
	sys_paths = ft_get_split_sys_paths(envp);
	i = 0;
	while (sys_paths && sys_paths[i])
	{
		complete_path = ft_path_join(sys_paths[i], cmd[0]);
		if (ft_verify_path(complete_path, 'e', DONT_PRINT_ERR))
		{
			free(cmd[0]);
			cmd[0] = complete_path;
			ft_fork_and_wait(cmd, envp);
			cmd_found = 1;
			break ;
		}
		free(complete_path);
		i++;
	}
	if (cmd_found == 0)
		printf("%s: command not found\n", cmd[0]);
	ft_splitfree(sys_paths);
}


void	ft_exec_cmd(char **cmd, char **envp)
{
	if (ft_strcmp(cmd[0], "exit") == 0)
		exit(0);
	else if (ft_strcmp(cmd[0], "cd") == 0)
		ft_cd(cmd, envp);
	else if (ft_strcmp(cmd[0], "env") == 0)
		ft_env(cmd, envp);
	else if (ft_strcmp(cmd[0], "setenv") == 0)
		ft_setenv(cmd, envp);
	else if (ft_strcmp(cmd[0], "unsetenv") == 0)
		ft_unsetenv(cmd, envp);
	else if (ft_chr_in_str('/', cmd[0]) && ft_verify_path(cmd[0], 'e', PRINT_ERR))
		ft_fork_and_wait(cmd, envp);
	else if (ft_chr_in_str('/', cmd[0]) == 0)
		ft_try_sys_path(cmd, envp);
}

int		main(int argc, char *argv[], char **envp)
{
	char	**cmds;
	char	**cmd;
	char	*line;
	int 	i;

	while (1)
	{
		ft_print_prompt();
		ft_gnl(0, &line);
		cmds = ft_strsplit(line, ';');
		i = 0;
		while (cmds[i] != NULL)
		{
			cmd = ft_strsplit(cmds[i], ' ');
			if (cmd[0] != NULL)
				ft_exec_cmd(cmd, envp);
			ft_splitfree(cmd);
			i++;
		}
		ft_splitfree(cmds);
		free(line);
	}
}