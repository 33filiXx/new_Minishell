/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 08:58:56 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/11 17:55:10 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

char	*strjoin_free_both(char *s1, char *s2)
{
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
	{
		joined = ft_strdup(s2);
		free(s2);
		return (joined);
	}
	if (!s2)
	{
		joined = ft_strdup(s1);
		free(s1);
		return (joined);
	}
	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

void	reset_info_expand(t_expand_var *expand)
{
	expand->check = 0;
	expand->edge = 0;
	expand->finale_r = ft_strdup("");
	expand->newstr = NULL;
	expand->str = NULL;
	expand->result = NULL;
	expand->i = 0;
	expand->lenght = 0;
}
void	reset_somme_info_expand(t_expand_var *expand)
{
	expand->check = 0;
	expand->edge = 0;
	expand->newstr = NULL;
	expand->str = NULL;
	expand->result = NULL;
	expand->i = 0;
	expand->lenght = 0;
	// expand->dollar = malloc(sizeof(t_dollar));
	// if (expand->dollar)
	// 	*(expand->dollar) = off;
}
void	reset_rebuild_var(t_rebuild_var *rebuild, char *lexer, char *env)
{
	rebuild->check = 0;
	rebuild->j = 0;
	rebuild->p = 0;
	rebuild->lenght_one = ft_strlen(lexer);
	rebuild->lenght_two = ft_strlen(env);
}

int	check_edge_cases(char **newstr, char **str, t_expand_var *expand,
		t_lexer **lexer)
{
	char	*tmp;
	int		i;
	int		stop;

	tmp = malloc(ft_strlen(*str) * sizeof(char *));
	if (!tmp)
		return (0);
	i = 0;
	stop = (*lexer)->lenght_normal;
	if (**str == '$' && *(*str + 1) && ft_isalpha(*(*str + 1)) != 1 && *(*str
			+ 1) != '?')
	{
		while (*str && ft_isalpha((*(*str))) != 1 && **str == '$' && *(*str
				+ 1) != '?')
		{
			if (**str == '$' && *(*str + 1) && ft_isalpha(*(*str + 1)))
			{
				break ;
			}
			if (*(*str) == '$' && !*(*str + 1))
			{
				tmp[i++] = **str;
				(*str)++;
				expand->lenght++;
				break ;
			}
			tmp[i++] = **str;
			(*str)++;
			expand->lenght++;
			if (expand->lenght == stop)
				break ;
		}
		tmp[i] = '\0';
		// printf("TMP : %s\n", tmp);
		*newstr = ft_strdup(tmp);
		free(tmp);
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
	if (**str == '$' && *(*str + 1) && *(*str + 1) != '?'
		&& (*lexer)->q[expand->i] != 22)
	{
		a = check_edge_cases(&newstr, str, expand, lexer);
		if (a == 1)
		{
			expand->edge = 1;
			return (newstr);
		}
	}
	// clean_up_helep(str, &newstr, expand, lexer);
	if (newstr)
		return (newstr);
	// *expand->dollar = off;
	return ("");
}

void	store_equal_env(t_expand_var *expand, char **str, t_lexer *lexer,
		t_env *env)
{
	t_env	*tmp_env;
	char	*to_cmp;
	int		stop;
	int		i;

	// printf("debage debage\n");
	// printf("STR : %s\n", *str);
	stop = 0;
	i = 0;
	to_cmp = malloc(ft_strlen(*str) + 1);
	if (*lexer->q == 0)
		stop = lexer->lenght_normal;
	tmp_env = env;
	// expand->lenght++;
	// (*str)++;
	if (*(*str + 1) && ft_isalpha(*(*str + 1)))
	{
		while (*str)
		{
			to_cmp[i++] = **str;
			(*str)++;
			if (!ft_isalpha(*(*str)) && expand->lenght < stop)
				break ;
			expand->lenght++;
		}
		to_cmp[i] = '\0';
	}
	// printf("tmp_vstr :  %s\n" , to_cmp);
	// printf("STR : %s\n", *str);
	// exit(1);
	while (tmp_env)
	{
		if (tmp_env->value && ft_strcmp(to_cmp + 1, tmp_env->key) == 0
			/*&& expand->check == on */)
		{
			expand->result = ft_strjoin(expand->result, tmp_env->value);
			free(to_cmp);
			to_cmp = NULL;
			break ;
		}
		tmp_env = tmp_env->next;
	}
	// printf("STR : %s\n", expand->result);
	// exit(1);
}

void	expand_logic_handler(t_expand_var *expand, t_lexer **lexer, char **p,
		t_env *env)
{
	char	one[2];

	while (**p)
	{
		if (**p == '$')
		{
			while (**p == '$' && *(*p + 1) && *(*p + 1) == '?')
			{
				(*p) += 2;
				expand->lenght += 2;
				expand->result = ft_strjoin(expand->result,
						ft_itoa((*exit_stat())));
			}
			expand->newstr = cleanup_argv(p, lexer, expand);
			if (expand->edge == 1)
				expand->result = ft_strjoin(expand->result, expand->newstr);
			if (**p == '$')
				store_equal_env(expand, p, *lexer, env);
			if (!ft_strcmp("$", *p))
			{
				expand->result = ft_strjoin(expand->result, *p);
				(*p)++;
				expand->lenght++;
			}
		}
		else
		{
			one[0] = **p;
			one[1] = '\0';
			expand->str = ft_strjoin(expand->result, one);
			expand->result = expand->str;
			(*p)++;
			expand->lenght++;
		}
		if (expand->lenght == (*lexer)->lenght_normal)
			break ;
	}
	printf("RESULT : %s\n", expand->result);
	exit(1);
}

void	search_comapre(t_env *env, t_lexer **lexer)
{
	char			*p;
	t_expand_var	*expand;

	(void)env;
	expand = malloc(sizeof(t_expand_var));
	reset_info_expand(expand);
	p = (*lexer)->content;
	while (*p)
	{
		if ((*lexer)->q[expand->i] == 0)
		{
			expand_logic_handler(expand, lexer, &p, env);
			expand->i++;
			reset_somme_info_expand(expand);
		}
		//if ((*lexer)->q[expand->i] == 1)
		//{
		//	expand_logic_handler(expand, lexer, &p, env);
		//}
	}
}

void	expand(t_env *env, t_lexer *lexer)
{
	while (lexer)
	{
		if (lexer->token != 2)
			search_comapre(env, &lexer);
		else
			lexer = lexer->next;
		lexer = lexer->next;
	}
}
