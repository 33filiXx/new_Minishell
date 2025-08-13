/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:26:06 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/13 04:27:08 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../nrc/minishell.h"

int	check_ambigoust(t_command *cmd, t_redirection *redir)
{
	if ((!redir->filename || ft_strcmp(redir->filename, "\0") == 0)
		&& cmd->redirs->type != 4)
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", 2);
		return (1);
	}
	else if (!redir->filename)
		return (1);
	else
		return (0);
}
