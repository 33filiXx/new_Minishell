/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:58:54 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/13 04:09:36 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	update_str_to_single(char **str)
{
	if (**str == '\'')
		(*str)++;
	while (**str)
	{
		if (**str == '\'')
			break ;
		(*str)++;
	}
}

void	r_and_check_double(t_lexer *lexer, t_store_in_q_var *var, char **str,
		t_store_helper store)
{
	if (store.state_double == 1)
	{
		lexer->lenght_double[var->j++] = count_double_lenght(*str);
		lexer->q[var->i++] = 1;
		update_str_to_double(str);
	}
	else if (store.state_double == 0)
	{
		lexer->lenght_edge[var->k++] = count_egde_lenght(*str);
		lexer->q[var->i++] = 2;
		update_edge_inside(str);
	}
}

int	count_single_length(char **str)
{
	int	counter;

	counter = 0;
	while (**str)
	{
		if (**str == '\'')
			break ;
		counter++;
		(*str)++;
	}
	if (**str == '\'')
		(*str)--;
	return (counter);
}

int	free_and_store_in_q(char *str, t_lexer *lexer)
{
	free(lexer->q);
	free(lexer->lenght_double);
	free(lexer->lenght_single);
	free(lexer->lenght_edge);
	lexer->q = malloc(ft_strlen(str) * sizeof(int));
	lexer->lenght_double = malloc(ft_strlen(str) * sizeof(int));
	if (!lexer->lenght_double)
		return (1);
	lexer->lenght_single = malloc(ft_strlen(str) * sizeof(int));
	if (!lexer->lenght_single)
		return (1);
	lexer->lenght_edge = malloc(ft_strlen(str) * sizeof(int));
	if (!lexer->lenght_edge)
		return (1);
	ft_bzero(lexer->q, ft_strlen(str) * sizeof(int));
	ft_bzero(lexer->lenght_double, ft_strlen(str) * sizeof(int));
	ft_bzero(lexer->lenght_single, ft_strlen(str) * sizeof(int));
	ft_bzero(lexer->lenght_edge, ft_strlen(str) * sizeof(int));
	return (0);
}

void	store_in_lexer_single(char *tmp, t_store_helper store, t_lexer *lexer,
		t_store_in_q_var *var)
{
	while (*tmp)
	{
		check_quotes_state(*tmp, &store);
		if (store.state_single == 1 && store.state_double == 0)
		{
			tmp++;
			lexer->lenght_single[var->p++] = count_single_length(&tmp);
			if (!*tmp)
				break ;
			check_quotes_state(*tmp, &store);
		}
		tmp++;
	}
}
