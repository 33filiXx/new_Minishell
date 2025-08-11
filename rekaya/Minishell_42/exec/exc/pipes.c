/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:12:37 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/05 15:08:26 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

int	count_commands(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	close_all_pipes(int **pipes, int count)
{
	int	i;

	i = 0;
	while (i < count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	cleanup_pipeline(int **pipes, int pipes_allocated, pid_t *pids,
			char **envp)
{
	int	i;

	i = 0;
	if (pipes)
	{
		while (i < pipes_allocated)
		{
			if (pipes[i])
				free(pipes[i]);
			i++;
		}
		free(pipes);
	}
	if (pids)
		free(pids);
	if (envp)
		free_split(envp);
}

int	execute_pipeline(t_command *cmd, t_env *env)
{
	t_pipeline_data	data;
	int				ret;

	if (setup_pipeline_data(cmd, env, &data))
		return (1);
	if (create_pipes(&data))
		return (1);
	if (fork_and_execute(cmd, env, &data))
		return (1);
	ret = wait_for_all_children(&data);
	cleanup_pipeline(data.pipes, data.count - 1, data.pids, data.envp);
	return (ret);
}
