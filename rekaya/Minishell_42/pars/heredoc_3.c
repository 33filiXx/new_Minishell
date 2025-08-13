/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 04:34:17 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/13 04:51:18 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

static void	update_lexer(char **str)
{
	if (**str == '$')
		(*str)++;
	while (**str)
	{
		if (ft_isalpha(**str) != 1)
			break ;
		(*str)++;
	}
}

int	alloc_rebuild(t_rebuild_var **rebuild, char *lexer, char *env,
		char **result)
{
	*rebuild = malloc(sizeof(t_rebuild_var));
	if (!(*rebuild))
		return (1);
	reset_rebuild_var(*rebuild, lexer, env);
	*result = malloc(((*rebuild)->lenght_one + (*rebuild)->lenght_two) + 1);
	if (!(*result))
	{
		free(*rebuild);
		return (1);
	}
	return (0);
}

char	*rebuild_and_stor(char **lexer, char *env)
{
	char			*result;
	t_rebuild_var	*rebuild;

	if (alloc_rebuild(&rebuild, *lexer, env, &result))
		return (NULL);
	while (**lexer)
	{
		if (**lexer == '$' && rebuild->check == 0)
		{
			while (env[rebuild->p])
				result[rebuild->j++] = env[rebuild->p++];
			update_lexer(lexer);
			rebuild->check = 1;
			continue ;
		}
		if (**lexer == '$' || **lexer == '\'' || **lexer == '"')
			break ;
		result[rebuild->j++] = *(*lexer)++;
	}
	result[rebuild->j] = '\0';
	free(rebuild);
	return (result);
}

void	update_to_dollar(char **str)
{
	if (**str == '$')
		(*str)++;
	while (**str)
	{
		if (ft_isalpha(**str) == 0)
			break ;
		(*str)++;
	}
}

void	update_exit(char **str)
{
	if (**str == '$')
		(*str)++;
	if (**str == '?')
		(*str)++;
}
