/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:07:40 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/19 16:24:50 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header/ft_pipex.h"
#include "printf/header/ft_printf.h"
#include "get_next_line/get_next_line_bonus.h"
#include <sys/types.h>
#include <sys/wait.h>

static void	ft_free_pipex(t_pipex *pipex)
{
	int	p;

	p = 0;
	if (!pipex)
		return ;
	if (pipex->path)
		free(pipex->path);
	if (pipex->command)
	{
		while (pipex->command[p])
		{
			free(pipex->command[p]);
			p++;
		}
		free(pipex->command);
	}
	free(pipex);
}

static void	ft_end(t_pipex *pipex)
{
	close(pipex->pipe[0][0]);
	close(pipex->pipe[0][1]);
	wait(NULL);
	ft_free_pipex(pipex);
}

int	ft_loop(t_pipex *pipex, char **env, int argc, char **argv)
{
	while (pipex->p < argc - 2)
	{
		wait(NULL);
		ft_clear_split(pipex->command);
		pipex->command = ft_split(argv[pipex->p], ' ');
		if (ft_path(env, &pipex, pipex->command[0]) == 0)
			return (ft_printf("error searching path\n"), 0);
		if (pipe(pipex->pipe[1]) == -1)
			return (perror("error opening pipe\n"), 0);
		if (ft_child(pipex->pipe[0][0], pipex->pipe[1][1], pipex, env) == 0)
			return (1);
		close(pipex->pipe[0][0]);
		close(pipex->pipe[0][1]);
		pipex->pipe[0][0] = pipex->pipe[1][0];
		pipex->pipe[0][1] = pipex->pipe[1][1];
	}
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;

	if (argc < 5)
		return (perror("error args\n"), 1);
	pipex = ft_parsing(argv, argc);
	if (!pipex)
		return (1);
	pipex->command = ft_split(argv[pipex->p], ' ');
	if (ft_path(env, &pipex, pipex->command[0]) == 0)
		return (ft_printf("error searching path\n"));
	if (pipe(pipex->pipe[0]) == -1)
		return (ft_printf("error opening pipe\n"), 1);
	if (ft_child(pipex->docs[0], pipex->pipe[0][1], pipex, env) == 0 ||
			(ft_loop(pipex, env, argc, argv) == 0))
		return (1);
	wait(NULL);
	ft_clear_split(pipex->command);
	pipex->command = ft_split(argv[argc - 2], ' ');
	if (ft_path(env, &pipex, pipex->command[0]) == 0)
		return (ft_printf("error searching path\n", 1));
	if (ft_child(pipex->pipe[0][0], pipex->docs[1], pipex, env) == 0)
		return (1);
	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")) == 0)
		unlink(argv[1]);
	return (ft_end(pipex), 0);
}
