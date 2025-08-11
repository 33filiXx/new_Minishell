/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:05:30 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/07 15:00:06 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

void	init_minimal_env(t_env **env)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
		set_env_value(env, "PWD", cwd);
	set_env_value(env, "SHLVL", "1");
	set_env_value(env, "_", "usr/bin/env");
}

void	print_env(t_env *env)
{
	while (env)
	{
		if (env->key && env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

t_env	*find_env_node(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	add_or_update_env(t_env **env_list, const char *key, const char *value)
{
	t_env	*node;

	if (!key || !env_list)
		return ;
	node = find_env_node(*env_list, key);
	if (node)
	{
		if (node->value)
			free(node->value);
		if (value != NULL)
			node->value = ft_strdup(value);
		else
			node->value = NULL;
		return ;
	}
	node = env_node_new(key, value);
	if (!node)
		return ;
	env_add_back(env_list, node);
}

void	update_shlvl(t_env **env_list)
{
	int		shlvl;
	char	*shlvl_str;
	t_env	*node;

	shlvl = 1;
	node = find_env_node(*env_list, "SHLVL");
	if (node && node->value)
	{
		shlvl = ft_atoi(node->value) + 1;
		if (shlvl < 0)
			shlvl = 0;
	}
	shlvl_str = ft_itoa(shlvl);
	if (shlvl_str)
	{
		add_or_update_env(env_list, "SHLVL", shlvl_str);
		free(shlvl_str);
	}
}
