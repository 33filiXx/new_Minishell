/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:58:54 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 19:01:10 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	reset_q_var(t_store_in_q_var *var)
{
	var->i = 0;
	var->j = 0;
	var->i = 0;
	var->p = 0;
}

int	handle_quotes_single(t_lexer *lexer, char **str, t_store_helper *store,
		t_store_in_q_var *var)
{
	lexer->q[var->i++] = 22;
	update_str_to_single(str);
	if (!*str)
		return (1);
	check_quotes_state(**str, store);
	return (0);
}

int	recall(t_store_helper *store, char *str ,t_store_in_q_var *var , t_lexer *lexer)
{
	reset_q_var(var);
	reset_data(store);
	if (free_and_store_in_q(str, lexer))
		exit(1);
	return (0);
}

void	store_in_q(char *str, t_lexer *lexer)
{
	t_store_helper		store;
	t_store_in_q_var	var;
	char				*tmp;

	tmp = str;
	recall(&store , str , &var , lexer);
	store_in_lexer_single(tmp, store, lexer, &var);
	reset_data(&store);
	while (*str)
	{
		check_quotes_state(*str, &store);
		if ((store.state_double == 1 || store.state_double == 0)
			&& store.state_single == 0)
		{
			r_and_check_double(lexer, &var, &str, store);
			if (!*str)
				break ;
			check_quotes_state(*str, &store);
		}
		else if (store.state_single == 1 && store.state_double == 0)
			if (handle_quotes_single(lexer, &str, &store, &var))
				break ;
		str++;
	}
	lexer->lenght_q = var.i;
}

void	get_last_node(t_lexer *lexer, char *str)
{
	while (lexer)
	{
		if (lexer->next == NULL)
			store_in_q(str, lexer);
		lexer = lexer->next;
	}
}
