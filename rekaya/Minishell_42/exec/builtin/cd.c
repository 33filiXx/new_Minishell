/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:47:19 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/09 22:25:27 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

static int	handle_chdir_error(char *target_path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(target_path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	return (1);
}

static void	handle_getcwd_error(void)
{
	ft_putstr_fd("cd: error retrieving current directory: ", 2);
	ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
	ft_putstr_fd("No such file or directory\n", 2);
}

static int	handle_cd_args(char **args)
{
	int count;

	if (!args || !*args)
		return (0);
	
	count = 0;
	while (args[count])
		count++;
	
	if (count > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

static char	*get_target_path(t_env *env, char **args)
{
	char	*home_path;

	if (args[1] && ft_strcmp(args[1], "\0") == 0)
		return (NULL);
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		home_path = get_env_value(env, "HOME");
		if (!home_path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (NULL);
		}
		return (home_path);
	}
	return (args[1]);
}

int	cd_builtin(t_env *env, char **args)
{
	char	*target_path;
	char	*old_path;
	char	*new_path;

	if (handle_cd_args(args))
		return (1);
	target_path = get_target_path(env, args);
	if (!target_path)
		return (1);
	old_path = getcwd(NULL, 0);
	if (chdir(target_path) == -1)
		return (free(old_path), handle_chdir_error(target_path));
	new_path = getcwd(NULL, 0);
	if (!new_path)
		handle_getcwd_error();
	else
	{
		set_env_value(&env, "OLDPWD", old_path);
		set_env_value(&env, "PWD", new_path);
		free(new_path);
	}
	free(old_path);
	return (0);
}
