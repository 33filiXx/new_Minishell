/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 08:58:56 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/11 12:27:36 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	free_dollar(t_dollar *d)
{
	if (!d)
		return ;
	// If d has malloc'd fields, free them here
	// Example:
	// free(d->key);
	// free(d->value);
	free(d);
}
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

void	free_expand_var(t_expand_var *var)
{
	if (!var)
		return ;
	// free(var->newstr);
	// free(var->result);
	// free(var->finale_r);
	// free(var->res);
	// free(var->str);
	if (var->dollar)
		free_dollar(var->dollar);
	free(var);
}

void	reset_info_expand(t_expand_var *expand)
{
	expand->check = 0;
	expand->edge = 0;
	expand->finale_r = ft_strdup("");
	expand->newstr = NULL;
	expand->str = NULL;
	expand->res = NULL;
	expand->result = NULL;
	expand->i = 0;
	expand->lenght = 0;
	expand->dollar = malloc(sizeof(t_dollar));
	if (expand->dollar)
		*(expand->dollar) = off;
}
void	reset_rebuild_var(t_rebuild_var *rebuild, char *lexer, char *env)
{
	rebuild->check = 0;
	rebuild->j = 0;
	rebuild->p = 0;
	rebuild->lenght_one = ft_strlen(lexer);
	rebuild->lenght_two = ft_strlen(env);
}

// char	*my_strdup(const char *s, t_lexer **lexer)
// {
// 	int		i;
// 	char	*p;

// 	if (!s)
// 		return (NULL);
// 	p = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
// 	if (!p)
// 		return (NULL);
// 	i = 0;
// 	// int lenght_double;
// 	while (i < (*((*lexer)->lenght_double)))
// 	{
// 		// if (ft_isalpha(s[i]) != 1)
// 		// 	break ;
// 		p[i] = s[i];
// 		i++;
// 	}
// 	p[i] = '\0';
// 	// printf("%s----\n", p);
// 	return (p);
// }

// int	alloc_rebuild(t_rebuild_var **rebuild, char *lexer, char *env,
// 		char **result)
// {
// 	*rebuild = malloc(sizeof(t_rebuild_var));
// 	if (!(*rebuild))
// 		return (1);
// 	reset_rebuild_var(*rebuild, lexer, env);
// 	*result = malloc(((*rebuild)->lenght_one + (*rebuild)->lenght_two) + 1);
// 	if (!(*result))
// 	{
// 		free(rebuild);
// 		return (1);
// 	}
// 	return (0);
// }

// char	*rebuild_and_stor(char **lexer, char *env, t_lexer *lx)
// {
// 	char			*result;
// 	t_rebuild_var	*rebuild;

// 	if (alloc_rebuild(&rebuild, *lexer, env, &result))
// 		return (NULL);
// 	// printf("%s----\n", *lexer);
// 	// exit(1);
// 	while (**lexer)
// 	{
// 		if (**lexer == '$' && rebuild->check == 0)
// 		{
// 			while (env[rebuild->p])
// 				result[rebuild->j++] = env[rebuild->p++];
// 			update_lexer(lexer, &lx);
// 			rebuild->check = 1;
// 			continue ;
// 		}
// 		if (**lexer == '$')
// 			break ;
// 		result[rebuild->j++] = *(*lexer)++;
// 	}
// 	result[rebuild->j] = '\0';
// 	free(rebuild);
// 	return (result);
// }

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
	stop = 0;
	if (*((*lexer)->q) == 0)
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
		printf("TMP : %s\n", tmp);
		*newstr = ft_strdup(tmp);
		*expand->dollar = on;
		free(tmp);
		return (1);
	}
	free(tmp);
	return (0);
}
// void	clean_up_helep(char *str, char **newstr, t_expand_var *expand,
// 		t_lexer **lexer)
// {
// 	int	i;

// 	i = 0;
// 	if (expand->i > (*lexer)->lenght_q)
// 		return ;
// 	if (str[i] == '$' && str[i + 1] != '\'' && str[i + 1] != '"' && str[i
// 		+ 1] != '\0' && (*lexer)->q[expand->i] != 22 && !(*newstr))
// 	{
// 		while (str[i])
// 		{
// 			if (str[i] == '$')
// 			{
// 				i++;
// 				if (str[i] == '?')
// 				{
// 					*newstr = ft_strdup("?");
// 					*expand->dollar = edge;
// 					break ;
// 				}
// 				if ((*lexer)->q[expand->i] == 1)
// 					*newstr = my_strdup(&str[i], lexer);
// 				else if ((*lexer)->q[expand->i] == 0)
// 					*newstr = my_strdup_case(&str[i]);
// 				*expand->dollar = on;
// 				break ;
// 			}
// 			i++;
// 		}
// 	}
// }
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
	*expand->dollar = off;
	return ("");
}

// void	store_equal_env(t_expand_var *expand, char **str, t_lexer *lexer,
// 		t_env *env)
// {
// 	t_env	*tm;

// 	tm = env;
// 	if (expand->i > lexer->lenght_q)
// 		return ;
// 	while (tm)
// 	{
// 		if (tm->value && ft_strcmp(expand->newstr, tm->key) == 0
// 			&& expand->check == on && lexer->q[expand->i] == 1)
// 		{
// 			expand->result = rebuild_and_stor(str, tm->value, lexer);
// 			expand->i += 1;
// 			expand->check = 1;
// 			free(expand->newstr);
// 			expand->newstr = NULL;
// 			break ;
// 		}
// 		tm = tm->next;
// 	}
// }

void	handle_normal_word(t_expand_var *expand, t_lexer **lexer, char **p)
{
	char	one[2];

	while (**p)
	{
		if (**p == '$')
		{
			expand->newstr = cleanup_argv(p, lexer, expand);
			if (expand->edge == 1)
				expand->result = ft_strjoin(expand->result, expand->newstr);
			printf("STR_plus : %s\n", *p);
		}
		else
		{
			one[0] = **p;
			one[1] = '\0';
			expand->str = ft_strjoin(expand->result, one);
			expand->result = expand->str;
		}
		(*p)++;
		expand->lenght++;
		if (expand->lenght == (*lexer)->lenght_normal)
			break ;
	}
	printf("NEW_STR : %s\n", expand->newstr);
	printf("RESULT : %s\n", expand->result);
	// printf("STR : %s\n", *p);
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
		// printf("dkhal hna\n");
		if ((*lexer)->q[expand->i] == 0)
			handle_normal_word(expand, lexer, &p);
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
