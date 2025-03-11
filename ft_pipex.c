/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 14:55:34 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/11 16:37:29 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header/ft_pipex.h"
#include "printf/header/ft_printf.h"
#include "get_next_line/get_next_line_bonus.h"
#include <sys/types.h>
#include <sys/wait.h>

void	ft_free_pipex(t_pipex *pipex)
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

void	ft_executor(int	read, int write, t_pipex *pipex, char **env)
{
	dup2(read, 0);
	dup2(write, 1);
	close(read);
	close(write);
	execve(ft_strjoin(pipex->path, ft_strjoin("/", pipex->command[0])), pipex->command, env);
	perror("error execve1\n");
	exit (0);
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;

	if (argc < 4)
		return (ft_printf("error args\n"), 1);
	pipex = ft_parsing(argv, argc);
	if (!pipex)
		return (1);
	pipex->command = ft_split(argv[2], ' ');
	if (ft_path(env, &pipex, pipex->command[0]) == 0)
		return (ft_printf("error searching path\n", 1));
	if (pipe(pipex->pipe[0]) == -1)
		return (ft_printf("error opening pipe\n"), 1);
	pipex->pid[0] = fork();
	if (pipex->pid[0] < 0)
		return (ft_printf("error fork\n"), 1);
	if (pipex->pid[0] == 0)
		ft_executor(pipex->docs[0], pipex->pipe[0][1], pipex, env);
	waitpid(pipex->pid[0], NULL, 0);
	ft_clear_split(pipex->command);
	pipex->command = ft_split(argv[3], ' ');
	if (ft_path(env, &pipex, pipex->command[0]) == 0)
		return (ft_printf("error searching path2\n", 1));
	pipex->pid[1] = fork();
	if (pipex->pid[1] < 0)
		return (ft_printf("error fork2\n"), 1);
	if (pipex->pid[1] == 0) //hijo
	{
		dup2(pipex->pipe[0][0], 0);
		dup2(pipex->docs[1], 1);
		close(pipex->pipe[0][0]);
		close(pipex->pipe[0][1]);
		close(pipex->docs[1]);
		execve(ft_strjoin(pipex->path, ft_strjoin("/", pipex->command[0])), pipex->command, env);
		perror("error execve2\n");
		exit (1);
	}
/*	if (pipex->pid[1] == 0)
		ft_executor(pipex->pipe[0][0], pipex->docs[1], pipex, env);*/
	close(pipex->pipe[0][1]);
	close(pipex->pipe[0][0]);
	waitpid(pipex->pid[0], NULL, 0);
	waitpid(pipex->pid[1], NULL, 0);
	ft_free_pipex(pipex);
	return (0);
}
