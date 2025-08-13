/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:34:17 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/13 05:04:12 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

char	*get_value(char *get)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	j = 0;
	line = malloc(ft_strlen(get) + 1);
	if (!line)
		return (NULL);
	while (get[i] && get[i] != '=')
		i++;
	i++;
	while (get[i])
		line[j++] = get[i++];
	line[j] = '\0';
	return (line);
}

int	lenght_of_lien(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] != ' ' && ft_isalpha(s[i]) == 1)
		i++;
	return (i);
}

char	*my_strdup(const char *s)
{
	int		i;
	char	*p;

	if (!s)
		return (NULL);
	p = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!p)
		return (NULL);
	i = 0;
	while (i < lenght_of_lien(s))
	{
		if (ft_isalpha(s[i]) != 1)
			break ;
		p[i] = s[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

void	clean_up_helep(char *str, char **newstr)
{
	int	i;

	i = 0;
	if (str[i] == '$' && str[i + 1] != '\'' && str[i + 1] != '"'
		&& str[i + 1] != '\0')
	{
		while (str[i])
		{
			if (str[i] == '$')
			{
				i++;
				if (str[i] == '?')
				{
					*newstr = ft_strdup("?");
					break ;
				}
				*newstr = my_strdup(&str[i]);
				break ;
			}
			i++;
		}
	}
}

char	*clean_argv(char *str)
{
	char	*newstr;

	newstr = NULL;
	clean_up_helep(str, &newstr);
	if (newstr)
		return (newstr);
	newstr = ft_strdup("\0");
	return (newstr);
}
