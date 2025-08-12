/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:17:05 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 17:23:31 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	handle_double_single(t_env *env, t_lexer **lexer, t_expand_var *expand,
			char **p)
{
	if ((*lexer)->q[expand->i] == 0)
	{
		expand_logic_handler(expand, lexer, p, env);
	}
	else if ((*lexer)->q[expand->i] == 1)
	{
		expand_logic_handler(expand, lexer, p, env);
		expand->i++;
		((*lexer)->lenght_double)++;
	}
	else if ((*lexer)->q[expand->i] == 2)
	{
		expand_logic_handler(expand, lexer, p, env);
		((*lexer)->lenght_edge)++;
		expand->i++;
	}
	else if ((*lexer)->q[expand->i] == 22)
	{
		expand_logic_sigle(expand, *lexer, p);
		((*lexer)->lenght_single)++;
		expand->i++;
	}
}

void	set_null_and_join(t_expand_var *expand)
{
	expand->finale_r = ft_strjoin(expand->finale_r, expand->result);
	expand->result = NULL;
	expand->newstr = NULL;
	expand->lenght = 0;
}

void	free_position(t_expand_norm *norm)
{
	free(norm->position_one);
	free(norm->position_two);
	free(norm->position_three);
	free(norm->d);
}

void	store_into_next(t_lexer **lexer, t_lexer *to_delete)
{
	(*lexer)->quotes = to_delete->quotes;
	(*lexer)->content = to_delete->content;
	(*lexer)->just_one_case = 1;
	(*lexer)->q = to_delete->q;
	(*lexer)->lenght_q = to_delete->lenght_q;
	(*lexer)->lenght_double = to_delete->lenght_double;
}

void	store_into_postion(t_expand_norm *norm, t_lexer **lexer)
{
	norm->position_one = (*lexer)->lenght_double;
	norm->position_two = (*lexer)->lenght_edge;
	norm->position_three = (*lexer)->lenght_single;
	norm->d = (*lexer)->q;
}
