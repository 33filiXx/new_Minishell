/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ex_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 22:10:18 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/05 15:06:09 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

int	handle_unset_command(t_command *cmd, t_env **env)
{
	int	i;

	i = 1;
	while (cmd->argv[i])
	{
		unset_env(env, cmd->argv[i]);
		i++;
	}
	return (0);
}

int	handle_exit_command(t_command *cmd, int abi)
{
	int	ret;

	ret = exit_builtin(cmd->argv, abi);
	if (ret == 0)
	{
		free_redirections(cmd->redirs);
		free_commend(cmd);
		exit(*exit_stat());
	}
	return (*exit_stat() = ret);
}

int	handle_redirection_only(t_command *cmd)
{
	int	save_stdin;
	int	save_stdout;
	int	ret;

	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	ret = redirections(cmd);
	dup2(save_stdin, STDIN_FILENO);
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdin);
	close(save_stdout);
	return (*exit_stat() = ret);
}

int	setup_builtin_redirections(t_command *cmd, int *save_stdin,
	int *save_stdout)
{
	int	ret;

	*save_stdin = dup(STDIN_FILENO);
	*save_stdout = dup(STDOUT_FILENO);
	ret = redirections(cmd);
	if (ret != 0)
	{
		dup2(*save_stdin, STDIN_FILENO);
		dup2(*save_stdout, STDOUT_FILENO);
		close(*save_stdin);
		close(*save_stdout);
		return (*exit_stat() = ret);
	}
	return (0);
}

void	restore_redirections(int save_stdin, int save_stdout)
{
	dup2(save_stdin, STDIN_FILENO);
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdin);
	close(save_stdout);
}
