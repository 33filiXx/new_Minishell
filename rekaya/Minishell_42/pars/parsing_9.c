/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_9.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:02:39 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/13 04:59:57 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

int	syntax_error_i_h(t_lexer *lexer)
{
	int	redirect;
	int	flage;

	redirect = check_redirection_h(lexer);
	flage = check_if_nofile_h(lexer);
	if ((redirect >= 1 && redirect <= 3) && flage != 0)
	{
		write(2, "minishell: syntax error near unexpected token `newline'\n",
			57);
		return (1);
	}
	if (redirect >= 6)
	{
		write(2, "minishell: syntax error near unexpected token `<<<'\n", 53);
		return (1);
	}
	else if (redirect == 5)
	{
		write(2, "minishell: syntax error near unexpected token `<<'\n", 52);
		return (1);
	}
	else if (redirect == 4)
		return (write(2, "minishell: \
				syntax error near unexpected token `<'\n", 51), 1);
	return (0);
}

int	check_pipes(t_lexer *lexer)
{
	int	count_of_pipes;
	int	count_res;

	count_res = 0;
	count_of_pipes = 0;
	while (lexer)
	{
		if (*lexer->content == '|' && lexer->quotes != 0)
			count_of_pipes += redirection_counter(lexer->content);
		else if (*lexer->content != '|')
		{
			if (count_of_pipes > count_res)
				count_res = count_of_pipes;
			count_of_pipes = 0;
		}
		lexer = lexer->next;
	}
	if (count_of_pipes > count_res)
		count_res = count_of_pipes;
	return (count_res);
}

int	check_if_out(t_lexer *lexer)
{
	while (lexer)
	{
		if (*lexer->content == '|' && lexer->quotes != 0 && !lexer->next)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}

int	syntax_error_pipes(t_lexer *lexer)
{
	int	pipes;
	int	flage;

	pipes = check_pipes(lexer);
	flage = check_if_out(lexer);
	if ((lexer && !ft_strcmp(lexer->content, "|") && pipes == 1 && flage != 1)
		|| (pipes == 1 && flage != 0))
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
		return (1);
	}
	if (pipes > 1)
	{
		write(2, "minishell: syntax error near unexpected token `||'\n", 52);
		return (1);
	}
	return (0);
}

int	lexer(char *argv, t_lexer **lexer)
{
	*lexer = NULL;
	store_into_nodes(argv, lexer);
	if (syntax_error_o_a(*lexer) == 1)
		return (1);
	if (syntax_error_pipes(*lexer) == 1)
		return (1);
	if (syntax_error_i_h(*lexer) == 1)
		return (1);
	if (!lexer)
		return (0);
	return (0);
}
