/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_8.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:34:17 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/13 04:55:07 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

int	alloc_file_buf(char **file, char **buf)
{
	*file = malloc(11);
	*buf = malloc(1);
	if (!*file || !*buf)
		return (1);
	return (0);
}

void	cleanup_namegen(int fd, char *file, char *buf)
{
	if (buf)
		free(buf);
	if (file)
		free(file);
	if (fd >= 0)
		close(fd);
}

int	fill_one_char(int fd, char *buf, char *file, int *j)
{
	int	valid_data;

	valid_data = read(fd, buf, 1);
	if (valid_data > 0)
	{
		if (ft_isascii(*buf) || *buf != '.')
			file[(*j)++] = *buf;
	}
	return (valid_data);
}

void	reassigne(t_heredoc *a, char *str, char **envp)
{
	a->clean_line = str;
	a->result = NULL;
	a->tmp = envp;
	a->i = 0;
}
