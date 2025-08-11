/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ex_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 22:12:55 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/09 22:25:29 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

char	**list_to_char_array(t_env *list)
{
	int		size;
	char	**array;
	int		i;
	char	*temp;
	char	*full_content;

	size = ft_lssize(list);
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (list)
	{
		temp = ft_strjoin(list->key, "=");
		if (list->value)
			full_content = ft_strjoin(temp, list->value);
		else
			full_content = ft_strdup(temp);
		free(temp);
		array[i] = full_content;
		list = list->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

int	ft_lssize(t_env *lst)
{
	int	counter;

	if (!lst)
		return (0);
	counter = 0;
	while (lst)
	{
		counter++;
		lst = lst->next;
	}
	return (counter);
}

static int	handle_env_and_exit(t_command *cmd, t_env **env, int abi)
{
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (handle_exit_command(cmd, abi));
	if (ft_strcmp(cmd->argv[0], "env") == 0)
	{
		if (cmd->argv[1])
			return (print_command_error(cmd->argv[1],
					" env: invalid option \n"), 125);
		print_env(*env);
		return (0);
	}
	return (0);
}

int	execute_builtin_commands(t_command *cmd, t_env **env)
{
	int	abi;

	abi = 0;
	if (!ft_strcmp(cmd->argv[0], "exit") && cmd->next)
		abi = 1;
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (cd_builtin(*env, cmd->argv));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
	{
		if (cmd->argv[1] && cmd->argv[1][0] == '-')
			return (print_command_error(cmd->argv[1],
					" pwd: invalid option \n"), 2);
		return (pwd_builtin(*env));
	}
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (echo_builtin(cmd->argv));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (export_builtin(env, cmd->argv));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (handle_unset_command(cmd, env));
	return (handle_env_and_exit(cmd, env, abi));
}

int	execute_external_command(t_command *cmd, t_env **env)
{
	char	**env_array;
	int		ret;

	env_array = list_to_char_array(*env);
	ret = excute_commend(cmd, env_array);
	free_split(env_array);
	*exit_stat() = ret;
	return (ret);
}
