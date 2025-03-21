/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 15:33:37 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/17 15:28:28 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		len;
	int		p;

	p = 0;
	if (!s1 && !s2)
		return (ft_strdup(""));
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	str = (char *)malloc(len * sizeof(char));
	if (!str)
		return (NULL);
	if (len <= 0)
		return (NULL);
	while (s1[p] != '\0')
	{
		str[p] = s1[p];
		p++;
	}
	ft_memmove((str + p), s2, (len - (size_t)p));
	return (str);
}

/*int	main()
{
	printf("%s", ft_strjoin("/", "wc"));
	return (0);
}*/
