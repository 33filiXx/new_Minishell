/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 08:58:56 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/12 16:56:59 by wel-mjiy         ###   ########.fr       */
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
	expand->result = NULL;
	expand->str = NULL;
	expand->lenght = 0;
}

void	store_int_logic_var(t_expand_var *expand, t_lexer **lexer, int *stop)
{
	if ((*lexer)->q[expand->i] == 0)
		*stop = (*lexer)->lenght_normal;
	else if ((*lexer)->q[expand->i] == 1)
		*stop = *((*lexer)->lenght_double);
	else if ((*lexer)->q[expand->i] == 2)
		*stop = *((*lexer)->lenght_edge);
}

int	check_edge_cases_helper(char **str, t_expand_var *expand, char *tmp, int *i)
{
	if (**str == '$' && *(*str + 1) && ft_isalpha(*(*str + 1)))
		return (1);
	if (*(*str) == '$' && !*(*str + 1))
	{
		tmp[(*i)++] = **str;
		(*str)++;
		expand->lenght++;
		return (1);
	}
	tmp[(*i)++] = **str;
	(*str)++;
	expand->lenght++;
	return (0);
}
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
	if (**str == '$' && *(*str + 1) && ft_isalpha(*(*str + 1)) != 1 && *(*str
			+ 1) != '?')
	{
		while (*str && ft_isalpha((*(*str))) != 1 && **str == '$' && *(*str
				+ 1) != '?')
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
	while (tmp_env)
	{
		if (tmp_env->value && ft_strcmp(to_cmp + 1, tmp_env->key) == 0)
		{
			expand->result = ft_strjoin(expand->result, tmp_env->value);
			free(to_cmp);
			to_cmp = NULL;
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
	if (*(*str + 1) && ft_isalpha(*(*str + 1)))
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
	}
	store_equal_env_helper(expand, to_cmp, tmp_env);
}

int	check_edge_one(t_expand_var *expand, int stop, char **p)
{
	if (**p == '$' && expand->lenght == stop - 1)
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
				expand->result = ft_strjoin(expand->result, expand->newstr);
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
		expand->result = expand->str;
		(*p)++;
		expand->lenght += 1;
		if (expand->lenght == stop)
			break ;
	}
}
void	handle_double_single(t_env *env, t_lexer **lexer, t_expand_var *expand,
		char **p)
{
	if ((*lexer)->q[expand->i] == 0)
	{
		expand_logic_handler(expand, lexer, p, env);
	}
	else if ((*lexer)->q[expand->i] == 1)
	{
		expand_logic_handler(expand, lexer, p, env);
		expand->i++;
		((*lexer)->lenght_double)++;
	}
	else if ((*lexer)->q[expand->i] == 2)
	{
		expand_logic_handler(expand, lexer, p, env);
		((*lexer)->lenght_edge)++;
		expand->i++;
	}
	else if ((*lexer)->q[expand->i] == 22)
	{
		expand_logic_sigle(expand, *lexer, p);
		((*lexer)->lenght_single)++;
		expand->i++;
	}
}
void	set_null_and_join(t_expand_var *expand)
{
	expand->finale_r = ft_strjoin(expand->finale_r, expand->result);
	expand->result = NULL;
	expand->newstr = NULL;
	expand->lenght = 0;
}

void	free_position(t_expand_norm *norm)
{
	free(norm->position_one);
	free(norm->position_two);
	free(norm->position_three);
	free(norm->d);
}
void	store_into_next(t_lexer **lexer, t_lexer *to_delete)
{
	(*lexer)->quotes = to_delete->quotes;
	(*lexer)->content = to_delete->content;
	(*lexer)->just_one_case = 1;
	(*lexer)->q = to_delete->q;
	(*lexer)->lenght_q = to_delete->lenght_q;
	(*lexer)->lenght_double = to_delete->lenght_double;
}
void	store_into_postion(t_expand_norm *norm, t_lexer **lexer)
{
	norm->position_one = (*lexer)->lenght_double;
	norm->position_two = (*lexer)->lenght_edge;
	norm->position_three = (*lexer)->lenght_single;
	norm->d = (*lexer)->q;
}
void	search_helper(t_expand_var *expand, t_lexer **lexer, t_lexer *to_delete,
		char **p)
{
	(*lexer)->next = to_delete->next;
	free(to_delete);
	*p = (*lexer)->content;
	reset_info_expand(expand);
}

void	store_in_var(t_expand_norm *norm, t_lexer *lexer)
{
	norm->d = lexer->q;
	norm->position_one = lexer->lenght_double;
	norm->position_two = lexer->lenght_edge;
	norm->position_three = lexer->lenght_single;
}

void	remove_node(t_expand_norm *norm, t_lexer **lexer, t_lexer *to_delete)
{
	free_position(norm);
	store_into_next(lexer, to_delete);
	store_into_postion(norm, lexer);
	
}

void	search_comapre(t_env *env, t_lexer **lexer)
{
	char			*p;
	t_expand_var	*expand;
	t_lexer			*to_delete;
	t_expand_norm	*norm;

	to_delete = malloc(sizeof(to_delete));
	norm = malloc(sizeof(t_expand_norm));
	store_in_var(norm, *lexer);
	expand = malloc(sizeof(t_expand_var));
	reset_info_expand(expand);
	p = (*lexer)->content;
	while (*p)
	{
		handle_double_single(env, lexer, expand, &p);
		if (!expand->result && (*lexer)->next)
		{
			to_delete = (*lexer)->next;
			remove_node(norm, lexer, to_delete);
			search_helper(expand, lexer, to_delete, &p);
		}
		else
			set_null_and_join(expand);
	}
	(*lexer)->content = ft_strdup(expand->finale_r);	
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
