/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 14:55:34 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/19 16:24:37 by lbellmas         ###   ########.fr       */
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
	pipex->path = NULL;
	pipex->command = NULL;
	free(pipex);
}

static void	ft_end(t_pipex *pipex)
{
	if (!pipex)
		return ;
	if (pipex->pipe[0][0] != 0)
		close(pipex->pipe[0][0]);
	if (pipex->pipe[0][1] != 0)
		close(pipex->pipe[0][1]);
	if (pipex->pipe[1][0] != 0)
		close(pipex->pipe[1][0]);
	if (pipex->pipe[1][1] != 0)
		close(pipex->pipe[1][1]);
	wait(NULL);
	ft_free_pipex(pipex);
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;

	if (argc != 5)
		return (ft_printf("error args\n"), 1);
	pipex = ft_parsing(argv, argc);
	if (!pipex)
		return (ft_end(pipex), 1);
	pipex->command = ft_split(argv[pipex->p], ' ');
	if (ft_path(env, &pipex, *pipex->command) == 0)
		return (ft_end(pipex), ft_printf("error searching path\n"));
	if (pipe(pipex->pipe[0]) == -1)
		return (ft_end(pipex), ft_printf("error opening pipe\n"));
	if (ft_child(pipex->docs[0], pipex->pipe[0][1], pipex, env) == 0)
		return (ft_end(pipex), 1);
	waitpid(pipex->pid, NULL, 0);
	ft_clear_split(pipex->command);
	pipex->command = ft_split(argv[pipex->p], ' ');
	if (ft_path(env, &pipex, pipex->command[0]) == 0)
		return (ft_end(pipex), ft_printf("error searching path\n"));
	if (ft_child(pipex->pipe[0][0], pipex->docs[1], pipex, env) == 0)
		return (ft_end(pipex), 1);
	return (ft_end(pipex), 0);
}
