/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:04:00 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/13 13:47:49 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_pipex.h"
#include "../printf/header/ft_printf.h"
#include "../get_next_line/get_next_line_bonus.h"
#include <sys/types.h>
#include <sys/wait.h>

int	ft_executor(int read, int write, t_pipex *pipex, char **env)
{
	char *temp;
	char *path_command;

	dup2(read, 0);
	dup2(write, 1);
	if (pipex->pipe[0][0] != read)
		close(pipex->pipe[0][0]);
	if (pipex->pipe[0][1] != write)
		close(pipex->pipe[0][1]);
	if (pipex->pipe[1][0] != read && pipex->pipe[1][0] != 0)
		close(pipex->pipe[1][0]);
	if (pipex->pipe[1][1] != write && pipex->pipe[1][1] != 0)
		close(pipex->pipe[1][1]);
	close(write);
	close(read);
	temp = ft_strjoin("/", pipex->command[0]);
	path_command = ft_strjoin(pipex->path, temp);
	free(temp);
	execve(path_command, pipex->command, env);
//	execve(ft_strjoin(pipex->path,
//			ft_strjoin("/", pipex->command[0])), pipex->command, env);
	perror("error execve1\n");
	return (0);
}

int	ft_child(int read, int write, t_pipex *pipex, char **env)
{
	pipex->pid = fork();
	if (pipex->pid < 0)
		return (ft_printf("error fork2\n"), 0);
	if (pipex->pid == 0)
	{
		if (ft_executor(read, write, pipex, env) == 0)
			return (0);
	}
	return (1);
}
