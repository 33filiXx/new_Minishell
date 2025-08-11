/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 21:56:05 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/07/29 18:42:09 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

void	print_command_error(char *cmd_name, char *error_msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(error_msg, 2);
}

void	check_path_exists(t_command *cmd)
{
	if (!cmd->path || (ft_strcmp(cmd->argv[0], ".") == 0)
		|| (ft_strcmp(cmd->argv[0], "\0") == 0))
	{
		print_command_error(cmd->argv[0], ": command not found\n");
		exit(127);
	}
}

void	check_file_access(t_command *cmd)
{
	struct stat	file_stat;

	if (access(cmd->path, F_OK) == -1)
	{
		print_command_error(cmd->argv[0], ": No such file or directory\n");
		exit(127);
	}
	if (stat(cmd->path, &file_stat) == 0
		&& S_ISDIR(file_stat.st_mode))
	{
		print_command_error(cmd->argv[0], ": Is a directory\n");
		exit(126);
	}
	if (access(cmd->path, X_OK) == -1)
	{
		print_command_error(cmd->argv[0], ": Permission denied\n");
		exit(126);
	}
}

void	handle_command_errors(t_command *cmd)
{
	check_path_exists(cmd);
	check_file_access(cmd);
}
