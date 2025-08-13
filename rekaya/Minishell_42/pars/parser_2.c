/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:36:59 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 18:37:53 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	change_pipe_value(t_command *command, int *check_out)
{
	command->pipe_in = 0;
	command->pipe_out = 0;
	*check_out = 1;
}

void	store_word_in_command(t_lexer **lexer, int *i, t_command *command,
		char **envp)
{
	while (*lexer && (*lexer)->token == WORD)
	{
		command->argv[*i] = ft_strdup((*lexer)->content);
		if (*i == 0)
			get_command_path(&command->argv[*i], envp, &command);
		(*i)++;
		*lexer = (*lexer)->next;
	}
	command->argv[*i] = NULL;
}

int	get_last_heredoc(t_lexer *lexer)
{
	t_lexer	*tmp;

	tmp = lexer;
	if (ft_strcmp("<<", tmp->content) == 0)
	{
		tmp = tmp->next;
	}
	while (tmp)
	{
		if (ft_strcmp("<<", tmp->content) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	count_heredoc(t_lexer *lexer)
{
	t_lexer	*tmp;
	int		counter;

	counter = 0;
	tmp = lexer;
	while (lexer)
	{
		if (!ft_strcmp(lexer->content, "<<"))
			counter += 1;
		lexer = lexer->next;
	}
	lexer = tmp;
	return (counter);
}

int	get_heredoc(t_lexer *lexer)
{
	t_lexer	*tmp;

	tmp = lexer;
	while (tmp)
	{
		if (ft_strcmp(tmp->content, "<<") == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
