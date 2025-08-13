/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:37:35 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/07/19 22:25:02 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

int	is_builtin_command(char *cmd_name)
{
	if (ft_strcmp(cmd_name, "cd") == 0 || ft_strcmp(cmd_name, "pwd") == 0
		|| ft_strcmp(cmd_name, "echo") == 0 || ft_strcmp(cmd_name,
			"export") == 0 || ft_strcmp(cmd_name, "unset") == 0
		|| ft_strcmp(cmd_name, "exit") == 0 || ft_strcmp(cmd_name, "env") == 0)
		return (1);
	return (0);
}

int	handle_initial_checks(t_command *cmd)
{
	if (!cmd)
		return (*exit_stat() = 0);
	if (!cmd->argv[0] && (cmd && cmd->redirs))
		return (handle_redirection_only(cmd));
	if (!cmd->argv[0])
		return (*exit_stat() = 0);
	return (-1);
}

int	handle_builtin_execution(t_command *cmd, t_env **env, int *save_stdin,
		int *save_stdout)
{
	int	ret;

	ret = setup_builtin_redirections(cmd, save_stdin, save_stdout);
	if (ret != 0)
		return (ret);
	ret = execute_builtin_commands(cmd, env);
	restore_redirections(*save_stdin, *save_stdout);
	return (ret);
}

int	execute_command_logic(t_command *cmd, t_env **env, int builtin)
{
	int	save_stdin;
	int	save_stdout;
	int	has_redir;

	has_redir = (cmd && cmd->redirs);
	if (builtin && has_redir)
		return (handle_builtin_execution(cmd, env, &save_stdin, &save_stdout));
	else if (builtin)
		return (execute_builtin_commands(cmd, env));
	else
		return (execute_external_command(cmd, env));
}

int	init_exc(t_command *cmd, t_env **env)
{
	int	ret;
	int	builtin;

	ret = handle_initial_checks(cmd);
	if (ret != -1)
		return (ret);
	builtin = is_builtin_command(cmd->argv[0]);
	ret = execute_command_logic(cmd, env, builtin);
	*exit_stat() = ret;
	return (ret);
}
