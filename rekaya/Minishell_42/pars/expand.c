/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 08:58:56 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/13 05:23:49 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

char	*strjoin_free_both(char *s1, char *s2)
{
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
	{
		joined = ft_strdup(s2);
		free(s2);
		return (joined);
	}
	if (!s2)
	{
		return (s1);
	}
	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

void	reset_info_expand(t_expand_var *expand)
{
	expand->check = 0;
	expand->edge = 0;
	expand->finale_r = ft_strdup("");
	expand->newstr = NULL;
	expand->str = NULL;
	expand->result = NULL;
	expand->i = 0;
	expand->lenght = 0;
}

void	reset_somme_info_expand(t_expand_var *expand)
{
	expand->check = 0;
	expand->edge = 0;
	expand->newstr = NULL;
	expand->result = NULL;
	expand->str = NULL;
	expand->lenght = 0;
}

void	store_int_logic_var(t_expand_var *expand, t_lexer **lexer, int *stop)
{
	if ((*lexer)->q[expand->i] == 0)
		*stop = (*lexer)->lenght_normal;
	else if ((*lexer)->q[expand->i] == 1)
		*stop = *((*lexer)->lenght_double);
	else if ((*lexer)->q[expand->i] == 2)
		*stop = *((*lexer)->lenght_edge);
}

int	check_edge_cases_helper(char **str, t_expand_var *expand, char *tmp, int *i)
{
	if (**str == '$' && *(*str + 1) && ft_isalpha(*(*str + 1)))
		return (1);
	if (*(*str) == '$' && !*(*str + 1))
	{
		tmp[(*i)++] = **str;
		(*str)++;
		expand->lenght++;
		return (1);
	}
	tmp[(*i)++] = **str;
	(*str)++;
	expand->lenght++;
	return (0);
}
