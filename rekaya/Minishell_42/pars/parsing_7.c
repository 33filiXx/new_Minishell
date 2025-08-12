/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:02:39 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/12 19:03:35 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	store_three_ht(char *str, int *i, t_lexer **lexer)
{
	char	*tmp;
	int		s;
	int		j;

	tmp = NULL;
	j = 0;
	s = 0;
	tmp = malloc(my_strlen(&str[*i]) + 1);
	if (!tmp)
		return ;
	s = *i;
	while (str[*i] && str[*i] != ' ' && str[*i] != '\t')
	{
		tmp[j++] = str[(*i)++];
		if (str[*i] == '|' || str[*i] == '>' || str[*i] == '<')
		{
			(*i)--;
			break ;
		}
	}
	tmp[j] = '\0';
	insert_at_end(lexer, tmp, WORD, 1);
	free(tmp);
	// tmp = NULL;
	get_last_normal(*lexer, str + s);
}

void	store_three(char *str, int *i, t_lexer **lexer)
{
	int	stalker;
	int	checker_quotes;

	stalker = 0;
	checker_quotes = 0;
	stalker = *i;
	store_three_ho(str, &checker_quotes, &stalker);
	if (checker_quotes == 0 && str[*i] != ' ' && str[*i] != '\t')
		store_three_ht(str, i, lexer);
	else if (checker_quotes == 1 && str[*i] != ' ')
		store_with_quotes(lexer, str, i, &checker_quotes);
}

void	store_into_nodes(char *str, t_lexer **lexer)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[i])
	{
		if (str[i] == '>' && str[i + 1] && str[i + 1] == '>')
			store_one(str, lexer, &i);
		else if (str[i] == '<' && str[i + 1] && str[i + 1] == '<')
			store_two(str, lexer, &i);
		else if (str[i] == '|')
			insert_at_end(lexer, "|", PIPE, 1);
		else if (str[i] == '>')
			insert_at_end(lexer, ">", TRUNC, 1);
		else if ((str[i++] == '<' && str[i] == '>') || str[--i] == '<')
			insert_at_end(lexer, "<", INPUT, 1);
		else if (str[i] && (str[i] != ' ' || str[i] != '\t'))
			store_three(str, &i, lexer);
		if (!str[i])
			break ;
		i++;
	}
}

int	edge_case_a(t_lexer *lexer)
{
	while (lexer)
	{
		if (ft_strcmp(lexer->content, ">") != 0)
			return (0);
		lexer = lexer->next;
	}
	return (1);
}
int	redirection_counter(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}
