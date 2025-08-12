/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:49:18 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 22:42:03 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	free_one_command(t_command *cmd)
{
	if (!cmd)
		return ;
	free_argv(cmd->argv);
	if (cmd->path)
		free(cmd->path);
	if (cmd->redirs)
		free_redi(cmd->redirs);
	free(cmd);
}

void	ft_free_command_list(t_command *head)
{
	t_command	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free_one_command(tmp);
	}
}

void	free_one_lexer(t_lexer *lexer)
{
	if (!lexer)
		return ;
	free(lexer->lenght_double);
	free(lexer->lenght_single);
	free(lexer->lenght_edge);
	free(lexer->content);
	free(lexer->q);
	free(lexer);
}

void	ft_free_lexer_list(t_lexer *head)
{
	t_lexer	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free_one_lexer(tmp);
	}
}

int	found_pipe(t_lexer *lexer)
{
	int	check;

	if (!lexer)
		return (1);
	check = 0;
	while (lexer)
	{
		if (lexer->token == PIPE)
		{
			check = 1;
			return (check);
		}
		lexer = lexer->next;
	}
	return (check);
}
