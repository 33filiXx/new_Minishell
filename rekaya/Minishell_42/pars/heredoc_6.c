/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:34:17 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/13 04:55:06 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	setup_parent_ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_child_signals(void)
{
	signal(SIGINT, sig_herdoc);
	signal(SIGQUIT, SIG_IGN);
}

void	child_process_write_filename(int write_fd, char **dof, char **envp,
		t_lexer **lexer)
{
	char	*filename;

	setup_child_signals();
	filename = heredoc_handle(dof, envp, lexer);
	if (filename)
	{
		write(write_fd, filename, ft_strlen(filename));
		free(filename);
	}
	close(write_fd);
	rl_clear_history();
	exit(0);
}

char	*read_filename_from_pipe(int read_fd)
{
	char	*filename;
	ssize_t	bytes_read;

	filename = malloc(1024);
	if (!filename)
		return (NULL);
	bytes_read = read(read_fd, filename, 1023);
	if (bytes_read > 0)
	{
		filename[bytes_read] = '\0';
		return (filename);
	}
	free(filename);
	return (NULL);
}
