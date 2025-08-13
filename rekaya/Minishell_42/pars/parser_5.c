/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:40:07 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/13 04:59:06 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

int	add_or_fail(t_command **command_list, t_command **command, t_lexer *lexer)
{
	*command = new_command_node();
	add_command_back(command_list, *command);
	(*command)->argv = malloc((sizeof(char *) * (count_argv(lexer) + 1)));
	if (!(*command)->argv)
	{
		*exit_stat() = 1;
		ft_free_command_list(*command_list);
		return (1);
	}
	return (0);
}

int	parser(t_lexer *lexer, t_command **command_list, char **envp)
{
	int			i;
	t_command	*command;
	t_lexer		*start;

	int (check_out), (check_pipe);
	command = NULL;
	start = lexer;
	*command_list = NULL;
	i = 0;
	check_pipe = 0;
	check_out = 0;
	while (lexer)
	{
		if (add_or_fail(command_list, &command, lexer) == 1)
			return (1);
		i = 0;
		if (found_pipe(lexer) == 0 && check_out == 0)
			change_pipe_value(command, &check_out);
		else
			check_out = 0;
		react_call(&lexer, command, &i, envp);
		change_pipe_checke_v(&lexer, command, &check_pipe);
	}
	lexer = start;
	return (0);
}
