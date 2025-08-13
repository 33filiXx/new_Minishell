/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:14:22 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/13 02:35:00 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

int	mini_helper(char *tmp, int i, char **newstr)
{
	tmp[i] = '\0';
	*newstr = ft_strdup(tmp);
	free(tmp);
	return (1);
}

int	check_edge_cases(char **newstr, char **str, t_expand_var *expand,
		t_lexer **lexer)
{
	char	*tmp;
	int		i;
	int		stop;

	i = 0;
	tmp = malloc(ft_strlen(*str) + 1);
	if (!tmp)
		return (0);
	store_int_logic_var(expand, lexer, &stop);
	if (**str == '$' && *(*str + 1) && ft_isalpha(*(*str + 1)) != 1
		&& *(*str + 1) != '?')
	{
		while (*str && ft_isalpha((*(*str))) != 1
			&& **str == '$' && *(*str + 1) != '?')
		{
			if (check_edge_cases_helper(str, expand, tmp, &i))
				break ;
			if (expand->lenght == stop)
				break ;
		}
		if (mini_helper(tmp, i, newstr))
			return (1);
	}
	free(tmp);
	return (0);
}

char	*cleanup_argv(char **str, t_lexer **lexer, t_expand_var *expand)
{
	int		a;
	char	*newstr;

	newstr = NULL;
	if (**str == '$' && *(*str + 1) && *(*str + 1) != '?')
	{
		a = check_edge_cases(&newstr, str, expand, lexer);
		if (a == 1)
		{
			expand->edge = 1;
			return (newstr);
		}
	}
	if (newstr)
		return (newstr);
	return ("");
}

void	store_equal_env_helper(t_expand_var *expand, char *to_cmp,
							t_env *tmp_env)
{
	char *tmp;
	while (tmp_env)
	{
		if (tmp_env->value && ft_strcmp(to_cmp + 1, tmp_env->key) == 0)
		{
			tmp = expand->result;
			expand->result = ft_strjoin(tmp, tmp_env->value);
			free(tmp);
			// free(to_cmp);
			// to_cmp = NULL;
			break ;
		}
		tmp_env = tmp_env->next;
	}
}

void	store_equal_env(t_expand_var *expand, char **str, t_lexer **lexer,
			t_env *env)
{
	t_env	*tmp_env;
	char	*to_cmp;
	int		stop;
	int		i;

	stop = 0;
	i = 0;
	to_cmp = malloc(ft_strlen(*str) + 1);
	store_int_logic_var(expand, lexer, &stop);
	tmp_env = env;
	if (*(*str + 1) && ft_isalpha(*(*str + 1)) && expand->lenght < stop)
	{
		while (*str)
		{
			to_cmp[i++] = **str;
			(*str)++;
			expand->lenght++;
			if (!ft_isalpha(*(*str)) || expand->lenght == stop)
				break ;
		}
		to_cmp[i] = '\0';
		store_equal_env_helper(expand, to_cmp, tmp_env);
	}
	free(to_cmp);
}
