/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:34:17 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/13 04:55:07 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	parent_wait_and_collect(pid_t pid, int read_fd, t_redirection **redir)
{
	int		status;
	char	*fname;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		*exit_stat() = 130;
		(*redir)->filename = NULL;
		close(read_fd);
		return ;
	}
	fname = read_filename_from_pipe(read_fd);
	if (fname)
		(*redir)->filename = fname;
	else
		(*redir)->filename = NULL;
	close(read_fd);
}

void	heredoc_parent(t_redirection **redir, char **dof, char **envp,
		t_lexer **lexer)
{
	pid_t	pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		return ;
	setup_parent_ignore_signals();
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		child_process_write_filename(pipefd[1], dof, envp, lexer);
	}
	else
	{
		close(pipefd[1]);
		parent_wait_and_collect(pid, pipefd[0], redir);
	}
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
