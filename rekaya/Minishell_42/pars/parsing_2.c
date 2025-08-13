/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:58:54 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/13 04:09:26 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	skip_empty_quotes(char *str, int *i)
{
	int	p;

	p = *i;
	while (str[p])
	{
		if (str[p] == '\'' && str[p + 1] == '\'')
			p += 2;
		else if (str[p] == '\"' && str[p + 1] == '\"')
			p += 2;
		else
			break ;
	}
	*i = p;
}

int	count_double_lenght(char *str)
{
	int	i;
	int	lenght;

	i = 0;
	lenght = 0;
	if (str[i] == '"')
		i++;
	while (str[i])
	{
		if (str[i] == '"')
			break ;
		lenght++;
		i++;
	}
	return (lenght);
}

int	count_egde_lenght(char *str)
{
	int	i;
	int	lenght;

	i = 0;
	lenght = 0;
	if (str[i] == '\"' || str[i] == '\'')
		i++;
	while (str[i])
	{
		lenght++;
		i++;
		if (str[i] == '"' || str[i] == '\'')
			break ;
	}
	return (lenght);
}

void	update_str_to_double(char **str)
{
	if (**str == '\"')
		(*str)++;
	while (**str)
	{
		if (**str == '"')
			break ;
		(*str)++;
	}
}

void	update_edge_inside(char **str)
{
	if (**str == '\"' || **str == '\'')
		(*str)++;
	while (**str)
	{
		if (**str == '"' || **str == '\'')
			break ;
		(*str)++;
	}
}
