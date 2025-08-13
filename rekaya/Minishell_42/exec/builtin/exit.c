/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:22:39 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/13 00:40:46 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

static int	validate_numeric_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '-' || arg[0] == '+')
		i = 1;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit(2);
		}
		i++;
	}
	return (1);
}

static int	normalize_exit_code(int exit_code)
{
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code = 256 + exit_code;
	return (exit_code);
}

int	exit_builtin(char **argv, int abi)
{
	int	exit_code;

	if (abi)
		printf("exit\n");
	if (!argv[1])
		exit(*exit_stat());
	validate_numeric_arg(argv[1]);
	if (argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	exit_code = ft_atoi(argv[1]);
	exit_code = normalize_exit_code(exit_code);
	exit(exit_code);
}
