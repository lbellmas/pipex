/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 15:22:47 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/19 16:24:21 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line/get_next_line_bonus.h"
#include "../printf/header/ft_printf.h"
#include "../header/ft_pipex.h"
#include <fcntl.h>

static int	ft_heredoc(char **argv)
{
	char	*str;
	int		temp;

	open(argv[1], O_CREAT, 0777);
	temp = open(argv[1], O_WRONLY);
	str = get_next_line(0);
	while (str && !(ft_strncmp(str, argv[2],
				ft_strlen(argv[2])) == 0 && str[ft_strlen(argv[2])] == '\n'))
	{
		if (write(temp, str, ft_strlen(str)) == -1)
			return (ft_printf("error in heredoc\n"), -1);
		free(str);
		str = get_next_line(0);
	}
	if (str)
		free(str);
	close(temp);
	return (3);
}

t_pipex	*ft_parsing(char **argv, int argc)
{
	t_pipex	*pipex;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	pipex->p = 2;
	if (ft_strncmp("here_doc", argv[1], 10) == 0 && argc > 5)
		pipex->p = ft_heredoc(argv);
	if (pipex->p == -1)
		return (free(pipex), NULL);
	if (access(argv[argc - 1], W_OK) != 0)
		return (free(pipex), (ft_printf("no permissions to write\n")), NULL);
	if (access(argv[argc - 1], F_OK) != 0)
		open(argv[argc - 1], O_CREAT, 0777);
	if (access(argv[1], F_OK) != 0)
		return (free(pipex), (ft_printf("the file does not exist\n")), NULL);
	if (access(argv[1], R_OK) != 0)
		return (free(pipex), (ft_printf("no permissions to read\n")), NULL);
	pipex->docs[0] = open(argv[1], O_RDONLY);
	pipex->docs[1] = open(argv[argc - 1], O_WRONLY);
	pipex->path = NULL;
	pipex->command = NULL;
	pipex->pipe[0][0] = 0;
	pipex->pipe[0][1] = 0;
	pipex->pipe[1][0] = 0;
	pipex->pipe[1][1] = 0;
	return (pipex);
}

char	*ft_find_path(char **env)
{
	int	p;

	p = 0;
	while (env[p])
	{
		if (ft_strncmp(env[p], "PATH=/", 6) == 0)
			return (env[p] + 6);
		p++;
	}
	return (NULL);
}

static int	ft_good_path(char *path, t_pipex **pipex, char **split, int p)
{
	free(path);
	(*pipex)->path = ft_strdup(split[p]);
	return (ft_clear_split(split), 1);
}

int	ft_path(char **env, t_pipex **pipex, char *cmd)
{
	char	**split;
	char	*temp;
	char	*path;
	int		p;

	if (!env || !*env)
		return (0);
	if ((*pipex)->path)
		free((*pipex)->path);
	split = ft_split(ft_find_path(env), ':');
	if (!split)
		return (0);
	p = 0;
	while (split[p])
	{
		temp = ft_strjoin("/", cmd);
		path = ft_strjoin(split[p], temp);
		free(temp);
		if (access(path, X_OK) == 0)
			return (ft_good_path(path, pipex, split, p));
		free(path);
		p++;
	}
	(*pipex)->path = NULL;
	return (ft_clear_split(split), 0);
}
