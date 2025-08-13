/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:34:17 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/13 04:39:29 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	update_exitstatus(t_heredoc *a)
{
	if (ft_strcmp(a->clean_line, "?"))
		update_to_dollar(&a->finale_line);
	else
	{
		a->result = ft_itoa((*exit_stat()));
		update_exit(&a->finale_line);
	}
}

int	send_a_tmp(t_heredoc *a)
{
	a->new_line = ft_newdup(a->tmp[a->i]);
	if (ft_strcmp(a->new_line, a->clean_line) == 0)
	{
		a->val = get_value(a->tmp[a->i]);
		a->result = rebuild_and_stor(&a->finale_line, a->val);
		free(a->val);
		free(a->new_line);
		return (1);
	}
	free(a->new_line);
	return (0);
}

int	character_copy(t_heredoc *a)
{
	if (*(a->finale_line) == '$')
		return (1);
	a->one[0] = *(a->finale_line);
	a->one[1] = '\0';
	a->tmp_join = ft_strjoin(a->result, a->one);
	free(a->result);
	a->result = a->tmp_join;
	return (0);
}

void	clear_buffer(t_heredoc *a)
{
	free(a->clean_line);
	a->tmp_join = ft_strjoin(a->finale_result, a->result);
	free(a->finale_result);
	free(a->result);
	a->finale_result = a->tmp_join;
}
