/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:02:39 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/13 04:59:24 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

int	check_(char *str, t_store_helper *store, int *p)
{
	if (store->state_double == 0 && store->state_single == 0)
		skip_empty_quotes(str, p);
	check_quotes_state(str[*p], store);
	if (go_out(store, str, p) == 1)
		return (1);
	helper_store(str, p, store);
	if (!str[*p])
		return (1);
	return (0);
}

char	*build_with_quotes(t_store_helper *store, char *str, int *p)
{
	int		j;
	char	*tmp;

	tmp = NULL;
	j = 0;
	tmp = malloc(sizeof(char) * (quotes_strlen(str + *p) + 1));
	if (!tmp)
		return (NULL);
	while (str[*p])
	{
		if (check_(str, store, p))
			break ;
		if ((str[*p] == '|' || str[*p] == '>' || str[*p] == '<')
			&& store->state_double == 0 && store->state_single == 0)
		{
			(*p)--;
			break ;
		}
		if ((str[*p] == ' ' || str[*p] == '\t') && store->state_double == 0
			&& store->state_single == 0)
			break ;
		tmp[j++] = str[(*p)++];
	}
	return (tmp[j] = '\0', tmp);
}

void	store_with_quotes(t_lexer **lexer, char *str, int *i,
		int *checker_quotes)
{
	int				p;
	char			*tmp;
	t_store_helper	store;

	tmp = NULL;
	p = *i;
	reset_data(&store);
	tmp = build_with_quotes(&store, str, &p);
	if (!tmp)
		return ;
	last_store(lexer, str, tmp, i);
	free(tmp);
	*i = p;
	*checker_quotes = 0;
}

void	store_one(char *str, t_lexer **lexer, int *i)
{
	insert_at_end(lexer, ">>", APPEND, 1);
	if (str[*i + 2] != ' ')
		*i = *i + 1;
	else
		*i = *i + 2;
}

void	store_two(char *str, t_lexer **lexer, int *i)
{
	insert_at_end(lexer, "<<", HERDOC, 1);
	if (str[*i + 2] != ' ')
		*i = *i + 1;
	else
		*i = *i + 2;
}
