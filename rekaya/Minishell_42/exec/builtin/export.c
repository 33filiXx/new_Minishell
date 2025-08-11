/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 20:27:30 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/07/19 16:40:22 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

void	equal_sign(char *arg, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		*key = ft_substr(arg, 0, equal - arg);
		*value = ft_strdup(equal + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}

void	plus_sign(t_env **env, char *arg, char **key, char **value)
{
	char	*equal;
	char	*exist_value;
	char	*new;
	char	*temp_exist;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		*key = ft_substr(arg, 0, equal - arg - 1);
		*value = ft_strdup(equal + 1);
		exist_value = get_env_value(*env, *key);
		if (exist_value)
		{
			temp_exist = ft_strdup(exist_value);
			new = ft_strjoin(temp_exist, *value);
			set_env_value(env, *key, new);
			free(temp_exist);
			free(new);
		}
		else
			set_env_value(env, *key, *value);
	}
}

static int	handle_plus_assignment(t_env **env, char *arg, int *ret)
{
	char	*key;
	char	*value;
	char	*plus;
	int		key_len;

	plus = ft_strnstr(arg, "+=", ft_strlen(arg));
	key_len = plus - arg;
	key = ft_substr(arg, 0, key_len);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("`: not a valid identifier\n", 2);
		free(key);
		*ret = 1;
		return (1);
	}
	free(key);
	plus_sign(env, arg, &key, &value);
	free(key);
	free(value);
	return (0);
}

static int	handle_regular_assignment(t_env **env, char *arg, int *ret)
{
	char	*key;
	char	*value;

	equal_sign(arg, &key, &value);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("`: not a valid identifier\n", 2);
		*ret = 1;
	}
	else
		set_env_value(env, key, value);
	free(key);
	free(value);
	return (0);
}

int	export_builtin(t_env **env, char **args)
{
	int		i;
	int		ret;

	i = 1;
	ret = 0;
	if (!args[1])
	{
		print_env_export(*env);
		return (0);
	}
	while (args[i])
	{
		if (ft_strnstr(args[i], "+=", ft_strlen(args[i])))
			handle_plus_assignment(env, args[i], &ret);
		else
			handle_regular_assignment(env, args[i], &ret);
		i++;
	}
	return (ret);
}
