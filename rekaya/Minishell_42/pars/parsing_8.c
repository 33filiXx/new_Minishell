/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_8.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:02:39 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 19:03:52 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

int	check_redirection_a(t_lexer *lexer)
{
	int	count_of_redirection;
	int	count_res;

	count_res = 0;
	count_of_redirection = 0;
	while (lexer)
	{
		if (*lexer->content == '>' && lexer->quotes != 0)
			count_of_redirection += redirection_counter(lexer->content);
		else if (*lexer->content != '>')
		{
			if (count_of_redirection > count_res)
				count_res = count_of_redirection;
			count_of_redirection = 0;
		}
		lexer = lexer->next;
	}
	if (count_of_redirection > count_res)
		count_res = count_of_redirection;
	return (count_res);
}

int	check_if_nofile_a(t_lexer *lexer)
{
	while (lexer)
	{
		if (*lexer->content == '>' && lexer->quotes != 0 && !lexer->next)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}

int	syntax_error_o_a(t_lexer *lexer)
{
	int	redirect;
	int	flage;

	redirect = check_redirection_a(lexer);
	flage = check_if_nofile_a(lexer);
	// printf("%d\n", edge_case_a(lexer));
	if (edge_case_a(lexer) == 1 && flage != 0 && redirect >= 2)
	{
		write(2, "minishell: syntax error near unexpected token `>'\n", 51);
		return (1);
	}
	if ((redirect == 1 || redirect == 2) && flage != 0)
	{
		write(2, "minishell: syntax error near unexpected token `newline'\n",
			57);
		return (1);
	}
	else if (redirect >= 4)
	{
		write(2, "minishell: syntax error near unexpected token `>>'\n", 51);
		return (1);
	}
	else if (redirect > 2)
	{
		write(2, "minishell: syntax error near unexpected token `>'\n", 51);
		return (1);
	}
	return (0);
}

int	check_redirection_h(t_lexer *lexer)
{
	int	count_of_redirection;
	int	count_res;

	count_res = 0;
	count_of_redirection = 0;
	while (lexer)
	{
		if (*lexer->content == '<' && lexer->quotes != 0)
			count_of_redirection += redirection_counter(lexer->content);
		else if (*lexer->content != '<')
		{
			if (count_of_redirection > count_res)
				count_res = count_of_redirection;
			count_of_redirection = 0;
		}
		lexer = lexer->next;
	}
	if (count_of_redirection > count_res)
		count_res = count_of_redirection;
	return (count_res);
}

int	check_if_nofile_h(t_lexer *lexer)
{
	while (lexer)
	{
		if (*lexer->content == '<' && lexer->quotes != 0 && !lexer->next)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}
