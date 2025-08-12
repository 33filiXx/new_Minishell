/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:58:54 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 19:01:51 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

int	size_normal_length(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && s[i] != ' ' && s[i] != '\t')
		i++;
	return (i);
}

void	get_last_normal(t_lexer *lexer, char *str)
{
	while (lexer)
	{
		if (lexer->next == NULL)
		{
			lexer->q[0] = 0;
			lexer->lenght_normal = size_normal_length(str);
		}
		lexer = lexer->next;
	}
}

void	helper_store(char *str, int *p, t_store_helper *store)
{
	if (!str || !str[*p])
		return ;
	if ((str[*p] == '\"' && store->state_double == 1
			&& store->state_single == 0) || store->last_quotes == 1)
	{
		(*p)++;
		if (store->check == 1)
			skip_empty_quotes(str, p);
		store->check = 1;
		if ((str[*p] == '\'' || str[*p] == '"') && store->last_quotes == 1)
			check_quotes_state(str[(*p)++], store);
		store->last_quotes = 0;
	}
	else if ((str[*p] == '\'' && store->state_double == 0
			&& store->state_single == 1) || store->last_quotes == 1)
	{
		(*p)++;
		if (store->check == 1)
			skip_empty_quotes(str, p);
		store->check = 1;
		if ((str[*p] == '\'' || str[*p] == '"') && store->last_quotes == 1)
			check_quotes_state(str[(*p)++], store);
		store->last_quotes = 0;
	}
}

void	last_store(t_lexer **lexer, char *str, char *tmp, int *i)
{
	insert_at_end(lexer, tmp, WORD, 0);
	get_last_node(*lexer, str + *i);
}

int	go_out(t_store_helper *store, char *str, int *p)
{
	if ((str[*p] == '|' || str[*p] == '>' || str[*p] == '<')
		&& store->state_double == 0 && store->state_single == 0)
	{
		(*p)--;
		return (1);
	}
	return (0);
}
