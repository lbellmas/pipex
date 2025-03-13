/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 15:22:47 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/13 16:24:03 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../printf/header/ft_printf.h"
#include "../header/ft_pipex.h"
#include <fcntl.h>

t_pipex	*ft_parsing(char **argv, int argc)
{
	t_pipex	*pipex;

	if (access(argv[1], F_OK) != 0)
		return ((ft_printf("no existe\n")), NULL);
	if (access(argv[1], R_OK) != 0)
		return ((ft_printf("no se puede leer\n")), NULL);
	if (access(argv[argc - 1], F_OK) != 0)
		open(argv[argc - 1], O_CREAT, 0777);
	if (access(argv[argc - 1], W_OK) != 0)
		return ((ft_printf("no se puede escribir\n")), NULL);
	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	pipex->docs[0] = open(argv[1], O_RDONLY);
	pipex->docs[1] = open(argv[argc - 1], O_WRONLY);
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

void	ft_clear_split(char **str)
{
	int	p;

	p = 0;
	while (str[p])
	{
		free(str[p]);
		p++;
	}
	free(str);
}

int	ft_path(char **env, t_pipex **pipex, char *cmd)
{
	char	**split;
	char	*temp;
	char	*path;
	int		p;

	if (!env || !*env)
		return (0);
	split = ft_split(ft_find_path(env), ':');
	if (!split)
		return (0);
	p = 0;
	while (split[p])
	{
		temp = ft_strjoin("/", cmd);
		path = ft_strjoin(split[p], temp);
		if (access(path, X_OK) == 0)
		{
			free(path);
			free(temp);
			(*pipex)->path = ft_strdup(split[p]);
			return (ft_clear_split(split), 1);
		}
		free(path);
		free(temp);
		p++;
	}
	return (ft_clear_split(split), 0);
}
