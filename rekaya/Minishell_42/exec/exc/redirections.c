/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:13:09 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 16:34:55 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

static int	handle_input_redirect(t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->filename);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_output_redirect(t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->filename);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_append_redirect(t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->filename);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_heredoc_redirect(t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->filename);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int    check_ambigoust(t_command *cmd, t_redirection    *redir)
{        
    if ((!redir->filename || ft_strcmp(redir->filename, "\0") == 0) && cmd->redirs->type != 4) 
    {
        ft_putstr_fd("minishell: ambiguous redirect\n", 2);
        return (1);
    }
    else if (!redir->filename)
        return (1);
    else
        return(0);
}

int    redirections(t_command *cmd)
{
    t_redirection    *redir;
    int                ret;

    ret = 0;
    redir = cmd->redirs;
    while (redir && ret == 0)
    {
        if (check_ambigoust(cmd, redir))
            return (1);
        if (redir->type == 1)
            ret = handle_input_redirect(redir);
        else if (redir->type == 2)
            ret = handle_output_redirect(redir);
        else if (redir->type == 3)
            ret = handle_append_redirect(redir);
        else if (redir->type == 4)
            ret = handle_heredoc_redirect(redir);
        redir = redir->next;
    }
    return (ret);
}
