/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:15:44 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 23:26:35 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

int	check_edge_one(t_expand_var *expand, int stop, char **p)
{
	if (**p == '$' && expand->lenght == stop)
	{
		expand->result = ft_strjoin(expand->result, "$");
		(*p)++;
		return (1);
	}
	return (0);
}

void	handle_exit_status(t_expand_var *expand, char **p)
{
	while (**p == '$' && *(*p + 1) && *(*p + 1) == '?')
	{
		(*p) += 2;
		expand->lenght += 2;
		expand->result = ft_strjoin(expand->result, ft_itoa((*exit_stat())));
	}
}

void	handle_without_dollar(t_expand_var *expand, char *one, char **p)
{
	one[0] = **p;
	one[1] = '\0';
	expand->str = ft_strjoin(expand->result, one);
	free(expand->result);
	expand->result = expand->str;
	(*p)++;
	expand->lenght += 1;
}

void	expand_logic_handler(t_expand_var *expand, t_lexer **lexer, char **p,
			t_env *env)
{
	int		stop;
	char	one[2];
	
	stop = 0;
	store_int_logic_var(expand, lexer, &stop);
	while (**p)
	{
		if (**p == '$')
		{
			handle_exit_status(expand, p);
			if (check_edge_one(expand, stop, p))
				break ;
			expand->newstr = cleanup_argv(p, lexer, expand);
			if (expand->edge == 1)
				expand->result = strjoin_free_both(expand->result, expand->newstr);
			if (**p == '$')
				store_equal_env(expand, p, lexer, env);
		}
		else
			handle_without_dollar(expand, one, p);
		if (expand->lenght == stop)
			break ;
	}
}

void	expand_logic_sigle(t_expand_var *expand, t_lexer *lexer, char **p)
{
	int		stop;
	char	one[2];

	stop = *lexer->lenght_single;
	while (**p)
	{
		one[0] = **p;
		one[1] = '\0';
		expand->str = ft_strjoin(expand->result, one);
		free(expand->result);
		expand->result = expand->str;
		(*p)++;
		expand->lenght += 1;
		if (expand->lenght == stop)
			break ;
	}
}
