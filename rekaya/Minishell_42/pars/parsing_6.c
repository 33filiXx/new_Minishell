/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:02:39 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 19:03:17 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	build_with_quotes(t_store_helper *store, char *str, int *p, char *tmp)
{
	int	j;

	j = 0;
	while (str[*p])
	{
		if (store->state_double == 0 && store->state_single == 0)
			skip_empty_quotes(str, p);
		check_quotes_state(str[*p], store);
		if (go_out(store, str, p) == 1)
			break ;
		helper_store(str, p, store);
		if (!str[*p])
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
	tmp[j] = '\0';
}

void	store_with_quotes(t_lexer **lexer, char *str, int *i,
		int *checker_quotes)
{
	int				p;
	char			*tmp;
	t_store_helper	store;
	int				len;

	p = *i;
	len = quotes_strlen(str + *i) + 1;
	tmp = malloc(sizeof(char) * (len));
	if (!tmp)
		return ;
	reset_data(&store);
	build_with_quotes(&store, str, &p, tmp);
	last_store(lexer, str, tmp, i);
	*i = p;
	*checker_quotes = 0;
	free(tmp);
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
void	store_three_ho(char *str, int *checker_quotes, int *stalker)
{
	while (str[*stalker] && str[*stalker] != ' ' && str[*stalker] != '\t')
	{
		if (str[*stalker] == '\'' || str[*stalker] == '"')
		{
			*checker_quotes = 1;
			break ;
		}
		(*stalker)++;
	}
}
