/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:20:04 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 23:39:50 by wel-mjiy         ###   ########.fr       */
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

void handle_extra(t_expand_var *expand , t_lexer **lexer , t_expand_norm *norm )
{
	if (ft_strcmp((*lexer)->content, "\0")
		&& !ft_strcmp(expand->finale_r, "\0"))
	{
		if ((*lexer)->content)
			free((*lexer)->content);
		(*lexer)->content = NULL;
	}
	else
	{
		free((*lexer)->content);
		(*lexer)->content = ft_strdup(expand->finale_r);
	}
	if (expand->finale_r)
	{
		free(expand->finale_r);
		expand->finale_r = NULL;
	}
	(*lexer)->lenght_double = norm->position_one;
	(*lexer)->lenght_edge = norm->position_two;
	(*lexer)->lenght_single = norm->position_three;
	(*lexer)->q = norm->d;
	free(expand->result);
	free(expand);
	free(norm);
}

void	search_comapre(t_env *env, t_lexer **lexer)
{
	char            *p;
	t_expand_var    *expand;
	t_lexer         *to_delete;
	t_expand_norm   *norm;

	to_delete = NULL;
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
	handle_extra(expand , lexer , norm);
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
