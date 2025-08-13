/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:34:17 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/13 05:03:20 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

char	*namegen(void)
{
	int		fd;
	char	*file;
	char	*buf;
	int		valid_data;
	int		j;

	fd = open_random_fd();
	if (fd < 0)
		return (NULL);
	if (alloc_file_buf(&file, &buf))
		return (cleanup_namegen(fd, NULL, NULL), NULL);
	valid_data = 0;
	j = 0;
	while (valid_data <= 0)
		valid_data = fill_one_char(fd, buf, file, &j);
	if (valid_data < 0)
		return (cleanup_namegen(fd, file, buf), NULL);
	file[j] = '\0';
	free(buf);
	close(fd);
	return (file);
}

char	*randgen(char *s)
{
	char	*pathname;
	char	*tmp;

	pathname = "/tmp/";
	tmp = namegen();
	s = ft_strjoin(pathname, tmp);
	free(tmp);
	return (s);
}

int	file_to_write_on(char **filename)
{
	char	*path;
	int		fd;

	path = randgen(*filename);
	fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0777);
	*filename = path;
	return (fd);
}

int	heredoc_strlen(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && s[i] != '=')
	{
		i++;
	}
	return (i);
}

char	*ft_newdup(const char *s)
{
	int		i;
	char	*p;

	if (!s)
		return (NULL);
	p = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!p)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != '=')
	{
		p[i] = s[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}
