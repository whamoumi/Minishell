/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztouzri <ztouzri@student.42nice.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 14:29:29 by ztouzri           #+#    #+#             */
/*   Updated: 2021/06/12 18:51:28 by ztouzri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(char *str, char charset)
{
	int	i;
	int	c;

	i = 0;
	c = 0;
	while (str[i] != 0)
	{
		while (str[i] == charset && str[i] != 0)
			i++;
		if (str[i] != charset && str[i] != 0)
			c++;
		while (str[i] != charset && str[i] != 0)
			i++;
	}
	return (c);
}

static void	ft_wordcpy(char *str, char charset, char *string, int i_str)
{
	int	i;

	i = 0;
	while (str[i_str] != charset && str[i_str] != 0)
	{
		string[i] = str[i_str];
		i++;
		i_str++;
	}
	string[i] = '\0';
}

static int	ft_skip(char *str, char charset, int i_str)
{
	while (str[i_str] == charset && str[i_str] != 0)
		i_str++;
	return (i_str);
}

static int	splitter(char *s, char c, char **string)
{
	int		i;
	int		j;
	int		i_str;

	i_str = 0;
	j = 0;
	i = 0;
	while (s[i_str] != 0
		&& ft_count(s, c) != 0 && s[i_str] != 0)
	{
		i = 0;
		i_str = ft_skip(s, c, i_str);
		while (s[i_str] != c
			&& s[i_str] != 0)
		{
			i++;
			i_str++;
		}
		string[j] = ft_calloc(sizeof (char), (i + 1));
		if (string[j] == 0)
			return (-1);
		ft_wordcpy((char *)s, c, string[j++], i_str - i);
		i_str = ft_skip((char *)s, c, i_str);
	}
	return (j);
}

char	**ft_split(char const *s, char c)
{
	char	**string;
	int		j;

	if (s == 0)
		return (0);
	j = 0;
	string = ft_calloc(sizeof (char *), (ft_count((char *)s, c) + 1));
	if (string == 0)
		return (0);
	j = splitter((char *)s, c, string);
	if (j == -1)
		return (0);
	return (string);
}
