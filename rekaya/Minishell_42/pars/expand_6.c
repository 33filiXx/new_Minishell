/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:20:04 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 18:57:29 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	search_helper(t_expand_var *expand, t_lexer **lexer, t_lexer *to_delete,
			char **p)
{
	(*lexer)->next = to_delete->next;
	free(to_delete);
	*p = (*lexer)->content;
	reset_info_expand(expand);
}

void	store_in_var(t_expand_norm *norm, t_lexer *lexer)
{
	norm->d = lexer->q;
	norm->position_one = lexer->lenght_double;
	norm->position_two = lexer->lenght_edge;
	norm->position_three = lexer->lenght_single;
}

void	remove_node(t_expand_norm *norm, t_lexer **lexer, t_lexer *to_delete)
{
	free_position(norm);
	store_into_next(lexer, to_delete);
	store_into_postion(norm, lexer);
}

void	search_comapre(t_env *env, t_lexer **lexer)
{
	char			*p;
	t_expand_var	*expand;
	t_lexer			*to_delete;
	t_expand_norm	*norm;

	to_delete = malloc(sizeof(to_delete));
	norm = malloc(sizeof(t_expand_norm));
	store_in_var(norm, *lexer);
	expand = malloc(sizeof(t_expand_var));
	reset_info_expand(expand);
	p = (*lexer)->content;
	while (*p)
	{
		handle_double_single(env, lexer, expand, &p);
		if (!expand->result && (*lexer)->next)
		{
			to_delete = (*lexer)->next;
			remove_node(norm, lexer, to_delete);
			search_helper(expand, lexer, to_delete, &p);
		}
		else
			set_null_and_join(expand);
	}
	if (ft_strcmp((*lexer)->content, "\0")
		&& !ft_strcmp(expand->finale_r, "\0"))
		(*lexer)->content = NULL;
	else
		(*lexer)->content = ft_strdup(expand->finale_r);
}

void	expand(t_env *env, t_lexer *lexer)
{
	while (lexer)
	{
		if (lexer->token != 2)
			search_comapre(env, &lexer);
		else
			lexer = lexer->next;
		lexer = lexer->next;
	}
}
