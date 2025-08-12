/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:39:20 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/13 00:01:30 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	redir_creation(t_redirection *redir, t_command *command,
			t_lexer **lexer, char **envp)
{
	char	**docs;
	int		i;

	i = 0;
	docs = ft_calloc((count_heredoc(*lexer) + 2), sizeof(char *));
	redir->next = NULL;
	if ((*lexer)->token == TRUNC)
	{
		redir->type = REDIR_OUT;
		redir->filename = ft_strdup((*lexer)->next->content);
	}
	else if ((*lexer)->token == HERDOC)
	{
		open_here_docs(redir, docs, lexer, &i);
		heredoc_parent(&redir, docs, envp, lexer);
	}
	else
	{
		track_redir(redir, lexer);
		redir->filename = ft_strdup((*lexer)->next->content);
	}
	add_and_move(command, redir, lexer);
	if (docs)
		free_split(docs);
}

int	store_in_redirect(t_lexer **lexer, t_command *command, char **envp)
{
	t_redirection	*redir;

	redir = NULL;
	while ((*lexer) && ((*lexer)->token == HERDOC
			|| (*lexer)->token == TRUNC
			|| (*lexer)->token == INPUT || (*lexer)->token == APPEND))
	{
		if (!(*lexer)->next || (*lexer)->next->token != WORD)
		{
			printf("minishell: syntax error near unexpected token `newline'\n");
			*exit_stat() = 2;
			break ;
		}
		redir = malloc(sizeof(t_redirection));
		if (!redir)
			return (*exit_stat() = 1);
		redir_creation(redir, command, lexer, envp);
	}
	return (*exit_stat());
}

int	count_argv(t_lexer *lexer)
{
	int	counter;

	counter = 0;
	while (lexer && lexer->token != PIPE)
	{
		if (lexer->token == WORD)
			counter++;
		lexer = lexer->next;
	}
	return (counter);
}

void	change_pipe_checke_v(t_lexer **lexer, t_command *command,
			int *check_pipe)
{
	if (*lexer && (*lexer)->token == PIPE)
	{
		if (*check_pipe == 0)
		{
			command->pipe_in = 0;
			command->pipe_out = 1;
			*check_pipe = 1;
		}
		else if (check_pipe != 0)
		{
			command->pipe_in = 1;
			command->pipe_out = 1;
		}
		*lexer = (*lexer)->next;
	}
}

void	react_call(t_lexer **lexer, t_command *command, int *i, char **envp)
{
	store_word_in_command(lexer, i, command, envp);
	store_in_redirect(lexer, command, envp);
	store_word_in_command(lexer, i, command, envp);
	store_in_redirect(lexer, command, envp);
}
