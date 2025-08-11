/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 19:50:02 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/09 21:53:27 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

int	wait_for_child(pid_t child_pid)
{
	int	status;

	if (child_pid <= 0)
		return (0);
	if (waitpid(child_pid, &status, 0) == -1)
		return (0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	handle_parent_process(pid_t pid1)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	status = wait_for_child(pid1);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	*exit_stat() = status;
	return (status);
}

void	execute_child_process(t_command *cmd, char **env)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->redirs)
	{
		if (redirections(cmd) != 0)
		{
			if (cmd->redirs->type != 4)
				exit(1);
			else 
				exit(130);
		}
	}
	handle_command_errors(cmd);
	if (execve(cmd->path, cmd->argv, env) == -1)
	{
		perror("execve failed");
		exit(127);
	}
}

int	excute_commend(t_command *cmd, char **env)
{
	pid_t	pid1;
	int		status;

	pid1 = fork();
	status = 0;
	if (pid1 < 0)
	{
		perror("fork failed");
		exit(1);
	}
	if (pid1 == 0)
		execute_child_process(cmd, env);
	else
		status = handle_parent_process(pid1);
	return (*exit_stat() = status);
}
