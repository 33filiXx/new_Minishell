/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:34:17 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/13 05:04:37 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

static t_heredoc	assigne_all(char **line)
{
	t_heredoc	test;

	test.new_line = NULL;
	test.finale_line = NULL;
	test.finale_line = *line;
	test.origin_line = *line;
	test.finale_result = NULL;
	test.clean_line = NULL;
	test.tmp = line;
	return (test);
}

void	special_case(t_heredoc *a)
{
	if (ft_strcmp(a->clean_line, "\0") == 0)
	{
		while (*(a->finale_line))
		{
			if (character_copy(a))
				break ;
			a->finale_line++;
		}
	}
	else
		update_exitstatus(a);
	clear_buffer(a);
}

void	expand_line(char **line, char **envp)
{
	t_heredoc	a;

	a = assigne_all(line);
	while (*(a.finale_line))
	{
		reassigne(&a, clean_argv(a.finale_line), envp);
		while (a.tmp[a.i])
		{
			if (send_a_tmp(&a))
				break ;
			a.i++;
		}
		special_case(&a);
	}
	free(a.origin_line);
	*line = a.finale_result;
}

static void	printer(char **dof, char *line, int fd, int i)
{
	if (!ft_strcmp(dof[i + 1], "\0"))
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
}

char	*heredoc_handle(char **dof, char **envp, t_lexer **lexer)
{
	char	*filename;
	char	*line;
	char	**tmp;

	int (i), (fd);
	tmp = dof;
	i = 0;
	fd = file_to_write_on(&filename);
	while (1)
	{
		line = readline("Heredoc >");
		if (line && ft_strcmp(line, dof[i]) == 0)
			i++;
		if (!line)
			return (dof = tmp, close(fd), filename);
		else if ((*lexer)->quotes == 1 && ft_strcmp(line, dof[i]))
			expand_line(&line, envp);
		if (!ft_strcmp(dof[i], "\0"))
			return (dof = tmp, close(fd), free(line), filename);
		printer(dof, line, fd, i);
		free(line);
	}
}
