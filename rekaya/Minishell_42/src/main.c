/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 22:00:19 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/13 05:19:33 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

static void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

static void	init_environment(t_env **env, char **envp)
{
	*env = init_env(envp);
	if (!*env)
		init_minimal_env(env);
	else
		update_shlvl(env);
}

void	free_and_reset_cmd(t_command **command)
{
	ft_free_command_list(*command);
	*command = NULL;
}

static void	run_shell(t_env *env, t_lexer *lexer_list, t_command *command)
{
	char	*input;

	lexer_list = NULL;
	command = NULL;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			ctrl_d_handle(env);
		if (*input)
			add_history(input);
		if (!check_syntax(input))
		{
			free(input);
			continue ;
		}
		if (lexer(input, &lexer_list))
		{
			ft_free_lexer_list(lexer_list);
			ft_free_command_list(command);
		}
		else
			handle_lexer_ok(env, &lexer_list, &command);
		free(input);
	}
}

int	main(int argc, char *argv[], char **envp)
{
	t_env		*env;
	t_lexer		*lexer_list;
	t_command	*command;

	lexer_list = NULL;
	command = NULL;
	(void)argv;
	(void)argc;
	setup_signals();
	init_environment(&env, envp);
	run_shell(env, lexer_list, command);
	write_history(".minishell_history");
	ft_free_env(env);
	return (*exit_stat());
}
