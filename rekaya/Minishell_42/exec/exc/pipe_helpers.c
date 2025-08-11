/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 21:20:00 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/06 14:22:26 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

int	setup_pipeline_data(t_command *cmd, t_env *env, t_pipeline_data *data)
{
	data->count = count_commands(cmd);
	data->envp = list_to_char_array(env);
	data->pipes = malloc(sizeof(int *) * (data->count - 1));
	if (!data->pipes)
		return (1);
	data->pids = malloc(sizeof(pid_t) * data->count);
	if (!data->pids)
	{
		free(data->pipes);
		free_split(data->envp);
		return (1);
	}
	return (0);
}

int	create_pipes(t_pipeline_data *data)
{
	int	i;

	i = 0;
	while (i < data->count - 1)
	{
		data->pipes[i] = malloc(sizeof(int) * 2);
		if (!data->pipes[i])
		{
			cleanup_pipeline(data->pipes, i, data->pids, data->envp);
			return (1);
		}
		if (pipe(data->pipes[i]) == -1)
		{
			perror("pipe");
			cleanup_pipeline(data->pipes, i + 1, data->pids, data->envp);
			return (1);
		}
		i++;
	}
	return (0);
}

void	child_process(t_command *cmd, t_pipeline_data *data, int i, t_env *env)
{
	int	ret;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->redirs)
	{
		if (redirections(cmd) != 0)
			exit(1);
	}
	if (i > 0)
		dup2(data->pipes[i - 1][0], STDIN_FILENO);
	if (i < data->count - 1)
		dup2(data->pipes[i][1], STDOUT_FILENO);
	close_all_pipes(data->pipes, data->count);
	if (!cmd->argv[0])
		exit(127);
	ret = init_exc(cmd, &env);
	*exit_stat() = ret;
	exit(ret);
}

int	fork_and_execute(t_command *cmd, t_env *env, t_pipeline_data *data)
{
	int			i;
	pid_t		pid;
	t_command	*current;

	i = 0;
	current = cmd;
	while (current)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork failed");
			cleanup_pipeline(data->pipes, data->count - 1, data->pids,
				data->envp);
			return (1);
		}
		else if (pid == 0)
			child_process(current, data, i, env);
		data->pids[i++] = pid;
		current = current->next;
	}
	return (0);
}

int	wait_for_all_children(t_pipeline_data *data)
{
	int	i;

	i = 0;
	close_all_pipes(data->pipes, data->count);
	signal(SIGINT, sigint_han_p);
	signal(SIGQUIT, SIG_IGN);
	while (i < data->count)
	{
		if (i == data->count - 1)
			*exit_stat() = wait_for_child(data->pids[i]);
		else
			wait_for_child(data->pids[i]);
		i++;
	}
	return (*exit_stat());
}
