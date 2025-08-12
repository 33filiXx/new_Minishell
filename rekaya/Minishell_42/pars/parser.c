/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:09:58 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/12 19:16:12 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

t_command	*new_command_node(void)
{
	t_command	*node;

	node = malloc(sizeof(t_command));
	if (!node)
		return (NULL);
	node->argv = NULL;
	node->redirs = NULL;
	node->path = NULL;
	node->pipe_in = 1;
	node->pipe_out = 0;
	node->next = NULL;
	return (node);
}

void	add_command_back(t_command **head, t_command *new)
{
	t_command	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	add_redir_back(t_redirection **head, t_redirection *new)
{
	t_redirection	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	free_argv(char **argv)
{
	int	i;

	i = 0;
	if (!argv)
		return ;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	free_redi(t_redirection *redir)
{
	t_redirection	*tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->filename)
			free(tmp->filename);
		free(tmp);
	}
}
