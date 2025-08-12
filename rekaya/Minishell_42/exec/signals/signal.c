/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:45:33 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 17:48:33 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

void	ctrl_d_handle(t_env *env) // ctrl d
{
	printf("exit\n");
	ft_free_env(env);
	exit(*exit_stat());
}

void	sigint_handler(int sig) // crtl c
{
	(void)sig;
	*exit_stat() = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_han_p(int sig) // crtl c
{
	(void)sig;
	*exit_stat() = 130;
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	sig_herdoc(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	*exit_stat() = 130;
	exit(130);
}