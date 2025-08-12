/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:09:58 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/12 05:25:51 by wel-mjiy         ###   ########.fr       */
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

void	free_redirs(t_redirection *redir)
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

void	free_one_command(t_command *cmd)
{
	if (!cmd)
		return ;
	free_argv(cmd->argv);
	if (cmd->path)
		free(cmd->path);
	if (cmd->redirs)
		free_redirs(cmd->redirs);
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
	// free(lexer->lenght_double);
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

void	free_docs(void)
{
	static char	**docs = NULL;
	static int	i = 0;

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

void	redir_creation(t_redirection *redir, t_command *command,
		t_lexer **lexer, char **envp)
{
	char	**docs = NULL;
	int	i;

	
	i = 0;
	docs = ft_calloc((count_heredoc(*lexer) + 1) , sizeof(char *));
	redir->filename = ft_strdup((*lexer)->next->content);
	redir->next = NULL;
	if ((*lexer)->token == TRUNC)
		redir->type = REDIR_OUT;
	else if ((*lexer)->token == 2)
	{
		// if (get_last_heredoc(*lexer) == 0)
		// {
			open_here_docs(redir, docs, lexer, &i);
			heredoc_parent(&redir, docs, envp, lexer);
		// }
		// if (get_last_heredoc(*lexer) == 1)
		// {
		// 	*lexer = (*lexer)->next;
		// 	docs[i++] = ft_strdup((*lexer)->content);
		// }
		// (*lexer) = (*lexer)->next; 	
	}
	else
		track_redir(redir, lexer);
	add_and_move(command, redir, lexer);
	free_split(docs);
}

int	store_in_redirect(t_lexer **lexer, t_command *command, char **envp)
{
	t_redirection	*redir;

	redir = NULL;
	while ((*lexer) && ((*lexer)->token == HERDOC || (*lexer)->token == TRUNC
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

int	add_or_fail(t_command **command_list, t_command **command, t_lexer *lexer)
{
	*command = new_command_node();
	add_command_back(command_list, *command);
	(*command)->argv = malloc((sizeof(char *) * (count_argv(lexer) + 1)));
	if (!(*command)->argv)
	{
		*exit_stat() = 1;
		ft_free_command_list(*command_list);
		return (1);
	}
	return (0);
}

int	parser(t_lexer *lexer, t_command **command_list, char **envp)
{
	int			i;
	t_command	*command;
	t_lexer		*start;

	int(check_out), (check_pipe);
	command = NULL;
	start = lexer;
	*command_list = NULL;
	i = 0;
	check_pipe = 0;
	check_out = 0;
	while (lexer)
	{
		if (add_or_fail(command_list, &command, lexer) == 1)
			return (1);
		i = 0;
		if (found_pipe(lexer) == 0 && check_out == 0)
			change_pipe_value(command, &check_out);
		else
			check_out = 0;
		react_call(&lexer, command, &i, envp);
		change_pipe_checke_v(&lexer, command, &check_pipe);
	}
	lexer = start;
	return (0);
}
