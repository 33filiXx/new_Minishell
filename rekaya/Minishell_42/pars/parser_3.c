/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:38:19 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 18:39:03 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	free_docs(void)
{
	static char	**docs;
	static int	i;

	docs = NULL;
	i = 0;
	if (docs)
	{
		while (i > 0)
		{
			i--;
			if (docs[i])
			{
				free(docs[i]);
				docs[i] = NULL;
			}
		}
		free(docs);
		docs = NULL;
	}
	i = 0;
}

void	open_here_docs(t_redirection *redir, char **docs, t_lexer **lexer,
			int *i)
{
	redir->type = HEREDOC;
	*lexer = (*lexer)->next;
	docs[(*i)++] = ft_strdup((*lexer)->content);
	docs[(*i)] = ft_strdup("\0");
}

void	track_rest(t_lexer **lexer)
{
	if ((*lexer) && (*lexer)->next && (*lexer)->token == WORD)
		(*lexer) = (*lexer)->next;
	else if ((*lexer) && !(*lexer)->next)
		(*lexer) = (*lexer)->next;
}

void	add_and_move(t_command *command, t_redirection *redir, t_lexer **lexer)
{
	add_redir_back(&command->redirs, redir);
	(*lexer) = (*lexer)->next;
	track_rest(lexer);
}

void	track_redir(t_redirection *redir, t_lexer **lexer)
{
	if ((*lexer)->token == APPEND)
		redir->type = REDIR_APPEND;
	else if ((*lexer)->token == INPUT)
		redir->type = REDIR_IN;
}
