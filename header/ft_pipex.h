/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 11:45:57 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/11 13:00:44 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FT_PIPEX_H
# define FT_PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_pipex
{
	int			docs[2];
	int			pipe[2][2];
	int			pid[2];
	char		**command;
	char		*path;
}	t_pipex;

t_pipex	*ft_parsing(char **argv, int argc);
char	*ft_find_path(char **env);
void	ft_clear_split(char **str);
int	ft_path(char **env, t_pipex **pipex, char *cmd);

#endif
