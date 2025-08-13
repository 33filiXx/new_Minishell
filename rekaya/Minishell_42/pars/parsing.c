/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:12:47 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/13 01:48:11 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	reset_data(t_store_helper *store)
{
	store->state_double = 0;
	store->state_single = 0;
	store->check = 0;
	store->last_quotes = 0;
}

int	my_strlen(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && s[i] != ' ')
	{
		i++;
	}
	return (i);
}

void	check_quotes_state(char str, t_store_helper *store)
{
	if (str == '\'' && store->state_double == 0)
		store->state_single = !(store->state_single);
	else if (str == '"' && store->state_single == 0)
		store->state_double = !(store->state_double);
	if (store->state_double == 0 && store->state_single == 0 && (str == '\''
			|| str == '"'))
		store->last_quotes = 1;
}
int	quotes_strlen(char *s)
{
	int			i;
	int			res;
	t_store_helper	state;

	i = 0;
	res = 0;
	if (!s)
		return (0);
	reset_data(&state);
	while (s[i])
	{
		check_quotes_state(s[i], &state);
		// stop at delimiters only when NOT inside quotes
		if (state.state_double == 0 && state.state_single == 0 &&
			(s[i] == ' ' || s[i] == '\t' || s[i] == '|' || s[i] == '>' || s[i] == '<'))
			break;
		res += 1;
		i++;
	}
	return (res);
}
int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1 && s2 && s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
	{
		i++;
	}
	return (s1[i] - s2[i]);
}
