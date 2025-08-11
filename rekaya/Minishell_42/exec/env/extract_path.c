/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:02:00 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/06 15:13:40 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

void	get_command_path(char **cmd, char **envp, t_command **command_list)
{
	if (cmd && cmd[0] && ft_strchr(cmd[0], '/'))
	{
		(*command_list)->path = ft_strdup(cmd[0]);
		if (!(*command_list)->path)
			return ;
	}
	else
		(*command_list)->path = extract_path(cmd[0], envp, command_list);
}

static	char	*join_path(char *path, char *cmd)
{
	char	*tmp;
	char	*fullpath;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	fullpath = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!fullpath)
		return (NULL);
	return (fullpath);
}

static char	*find_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == NULL)
		i++;
	if (envp[i] == NULL)
		return (NULL);
	return (envp[i] + 5);
}

static char	*search_in_paths(char **path, char *cmd)
{
	char	*fullpath;
	int		i;

	i = 0;
	while (path && path[i])
	{
		fullpath = join_path(path[i], cmd);
		if (access(fullpath, X_OK) == 0)
			return (fullpath);
		free(fullpath);
		i++;
	}
	return (NULL);
}

char	*extract_path(char *cmd, char **envp, t_command **command_list)
{
	char	**path;
	char	*fullpath;
	char	*path_env;

	if (!command_list || !(*command_list))
		return (NULL);
	path_env = find_path_env(envp);
	if (!path_env)
		return (NULL);
	path = ft_split(path_env, ':');
	fullpath = search_in_paths(path, cmd);
	free_split(path);
	return (fullpath);
}
