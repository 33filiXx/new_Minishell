/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:12:47 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/12 04:12:35 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void	reset_data(t_store_helper *store)
{
	store->state_double = 0;
	store->state_single = 0;
	store->check = 0;
	store->last_quotes = 0;
}

int	my_strlen(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && s[i] != ' ')
	{
		i++;
	}
	return (i);
}
void	check_quotes_state(char str, t_store_helper *store)
{
	if (str == '\'' && store->state_double == 0)
		store->state_single = !(store->state_single);
	else if (str == '"' && store->state_single == 0)
		store->state_double = !(store->state_double);
	if (store->state_double == 0 && store->state_single == 0 && (str == '\''
			|| str == '"'))
		store->last_quotes = 1;
}
int	quotes_strlen(char *s)
{
	int				i;
	int				res;
	t_store_helper	state;

	i = 0;
	res = 0;
	if (!s)
		return (0);
	reset_data(&state);
	while (s[i])
	{
		check_quotes_state(s[i], &state);
		if (state.state_double == 0 && state.state_double == 0 && (s[i] == ' '
				|| s[i] == '\t' || !s[i]))
			break ;
		res += 1;
		i++;
	}
	return (res);
}
int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1 && s2 && s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
	{
		i++;
	}
	return (s1[i] - s2[i]);
}
int	my_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
	{
		i++;
	}
	return (i);
}
void	ft_free(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
	ptr = NULL;
}
void	check_helper_one(t_lexer *lexer)
{
	printf("bash: syntax error near unexpected token `newline'\n");
	ft_free_nodes(lexer);
	exit(1);
}

char	check_sep(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			break ;
		i++;
	}
	return (str[i]);
}

void	skip_empty_quotes(char *str, int *i)
{
	int	p;

	p = *i;
	while (str[p])
	{
		if (str[p] == '\'' && str[p + 1] == '\'')
			p += 2;
		else if (str[p] == '\"' && str[p + 1] == '\"')
			p += 2;
		else
			break ;
	}
	*i = p;
}

int	finde_dollar(char str)
{
	if (str == '$')
		return (1);
	return (0);
}
int	finde_edge_quote(char str)
{
	if (!str)
		return (0);
	if (str == '"' || str == '\'')
		return (1);
	return (0);
}
int	count_double_lenght(char *str)
{
	int	i;
	int	lenght;

	i = 0;
	lenght = 0;
	if (str[i] == '"')
		i++;
	while (str[i])
	{
		if (str[i] == '"')
			break ;
		lenght++;
		i++;
	}
	// if (lenght == 2)
	// 	lenght--;
	return (lenght);
}
int	count_egde_lenght(char *str)
{
	int	i;
	int	lenght;

	i = 0;
	lenght = 0;
	if (str[i] == '\"' || str[i] == '\'')
		i++;
	while (str[i])
	{
		lenght++;
		i++;
		if (str[i] == '"' || str[i] == '\'')
			break ;
	}
	// if (lenght == 2)
	// 	lenght--;
	// printf("%d" , lenght);
	// exit(1);
	return (lenght);
}

void	update_str_to_double(char **str)
{
	if (**str == '\"')
		(*str)++;
	while (**str)
	{
		if (**str == '"')
			break ;
		(*str)++;
	}
	if (**str == '"')
		(*str)--;
}
void	update_edge_inside(char **str)
{
	if (**str == '\"' || **str == '\'')
		(*str)++;
	while (**str)
	{
		if (**str == '"' || **str == '\'')
			break ;
		(*str)++;
	}
	if (**str == '"' || **str == '\'')
		(*str)--;
}

void	update_str_to_single(char **str)
{
	if (**str == '\'')
		(*str)++;
	while (**str)
	{
		if (**str == '\'')
			break ;
		(*str)++;
	}
	if (**str == '\'')
		(*str)--;
}

int	r_and_check_double(t_lexer *lexer, int *j, char **str, int *i,
		t_store_helper store, int *k)
{
	// printf("%s\n" , str);
	if (store.state_double == 1)
	{
		// printf("double {%s}\n", *str);
		lexer->lenght_double[(*j)++] = count_double_lenght(*str);
		lexer->q[(*i)++] = 1;
		update_str_to_double(str);
	}
	else if (store.state_double == 0 )
	{
		// printf("egde [{%s}]\n", *str);
		lexer->lenght_edge[(*k)++] = count_egde_lenght(*str);
		lexer->q[(*i)++] = 2;
		update_edge_inside(str);
	}
	return (0);
}
int	count_single_length(char **str)
{
	int	counter;

	counter = 0;
	while (**str)
	{
		if (**str == '\'')
			break ;
		counter++;
		(*str)++;
	}
	if (**str == '\'')
		(*str)--;
	return (counter);
}
void	store_in_q(char *str, t_lexer *lexer)
{
	t_store_helper	store;
	int				i;
	int				j;
	int				p;
	int				k;
	char			*tmp;

	tmp = str;
	reset_data(&store);
	i = 0;
	j = 0;
	p = 0;
	k = 0;
	free(lexer->q);
	free(lexer->lenght_double);
	free(lexer->lenght_single);
	free(lexer->lenght_edge);
	lexer->q = malloc(ft_strlen(str) * sizeof(int));
	lexer->lenght_double = malloc(ft_strlen(str) * sizeof(int));
	lexer->lenght_single = malloc(ft_strlen(str) * sizeof(int));
	lexer->lenght_edge = malloc(ft_strlen(str) * sizeof(int));
	ft_bzero(lexer->q, ft_strlen(str) * sizeof(int));
	ft_bzero(lexer->lenght_double, ft_strlen(str) * sizeof(int));
	ft_bzero(lexer->lenght_single, ft_strlen(str) * sizeof(int));
	ft_bzero(lexer->lenght_edge, ft_strlen(str) * sizeof(int));
	while (*tmp)
	{
		check_quotes_state(*tmp, &store);
		if (store.state_single == 1 && store.state_double == 0)
		{
			tmp++;
			lexer->lenght_single[p++] = count_single_length(&tmp);
			if (!*tmp)
				break ;
		}
		tmp++;
	}
	tmp = str;
	reset_data(&store);
	while (*str)
	{
		// printf(" %d === %d " , store.state_double , store.state_single);
		check_quotes_state(*str, &store);
		// printf(" %d === %d " , store.state_double , store.state_single);
		// exit(1);
		if ((store.state_double == 1 || store.state_double == 0)
			 && store.state_single == 0)
		{
			// printf("%s\n", str);
			// exit(1);
			if (r_and_check_double(lexer, &j, &str, &i, store, &k))
				return ;
			// printf("\n");
			if (!*str)
				break ;
		}
		else if (store.state_single == 1 && store.state_double == 0)
		{
			lexer->q[i++] = 22;
			update_str_to_single(&str);
			if (!*str)
				break ;
		}
		str++;
	}
	// lexer->q[i] = 0;
	lexer->lenght_q = i;
}

void	get_last_node(t_lexer *lexer, char *str)
{
	while (lexer)
	{
		if (lexer->next == NULL)
			store_in_q(str, lexer);
		lexer = lexer->next;
	}
}

int	size_normal_length(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && s[i] != ' ' && s[i] != '\t')
		i++;
	return (i);
}

void	get_last_normal(t_lexer *lexer, char *str)
{
	// printf("%s\n" , str);
	while (lexer)
	{
		if (lexer->next == NULL)
		{
			lexer->q[0] = 0;
			lexer->lenght_normal = size_normal_length(str);
			// printf("%d +++" , lexer->lenght_normal);
		}
		lexer = lexer->next;
	}
}

void	helper_store(char *str, int *p, t_store_helper *store)
{
	if (!str || !str[*p])
		return ;
	if ((str[*p] == '\"' && store->state_double == 1
			&& store->state_single == 0) || store->last_quotes == 1)
	{
		(*p)++;
		if (store->check == 1)
			skip_empty_quotes(str, p);
		store->check = 1;
		if ((str[*p] == '\'' || str[*p] == '"') && store->last_quotes == 1)
			check_quotes_state(str[(*p)++], store);
		store->last_quotes = 0;
	}
	else if ((str[*p] == '\'' && store->state_double == 0
			&& store->state_single == 1) || store->last_quotes == 1)
	{
		(*p)++;
		if (store->check == 1)
			skip_empty_quotes(str, p);
		store->check = 1;
		if ((str[*p] == '\'' || str[*p] == '"') && store->last_quotes == 1)
			check_quotes_state(str[(*p)++], store);
		store->last_quotes = 0;
	}
}

void	last_store(t_lexer **lexer, char *str, char *tmp, int *i)
{
	insert_at_end(lexer, tmp, WORD, 0);
	get_last_node(*lexer, str + *i);
}

int	go_out(t_store_helper *store, char *str, int *p)
{
	if ((str[*p] == '|' || str[*p] == '>' || str[*p] == '<')
		&& store->state_double == 0 && store->state_single == 0)
	{
		(*p)--;
		return (1);
	}
	return (0);
}

void	build_with_quotes(t_store_helper *store, char *str, int *p, char *tmp)
{
	int	j;

	j = 0;
	while (str[*p])
	{
		if (store->state_double == 0 && store->state_single == 0)
			skip_empty_quotes(str, p);
		check_quotes_state(str[*p], store);
		if (go_out(store, str, p) == 1)
			break ;
		helper_store(str, p, store);
		if (!str[*p])
			break ;
		if ((str[*p] == '|' || str[*p] == '>' || str[*p] == '<')
			&& store->state_double == 0 && store->state_single == 0)
		{
			(*p)--;
			break ;
		}
		if ((str[*p] == ' ' || str[*p] == '\t') && store->state_double == 0
			&& store->state_single == 0)
			break ;
		tmp[j++] = str[(*p)++];
	}
	tmp[j] = '\0';
}

void	store_with_quotes(t_lexer **lexer, char *str, int *i,
		int *checker_quotes)
{
	int				p;
	char			*tmp;
	t_store_helper	store;
	int				len;

	p = *i;
	len = quotes_strlen(str + *i) + 1;
	tmp = malloc(sizeof(char) * (len));
	if (!tmp)
		return ;
	reset_data(&store);
	build_with_quotes(&store, str, &p, tmp);
	last_store(lexer, str, tmp, i);
	*i = p;
	*checker_quotes = 0;
	free(tmp);
}

void	store_one(char *str, t_lexer **lexer, int *i)
{
	insert_at_end(lexer, ">>", APPEND, 1);
	if (str[*i + 2] != ' ')
		*i = *i + 1;
	else
		*i = *i + 2;
}
void	store_two(char *str, t_lexer **lexer, int *i)
{
	insert_at_end(lexer, "<<", HERDOC, 1);
	if (str[*i + 2] != ' ')
		*i = *i + 1;
	else
		*i = *i + 2;
}
void	store_three_ho(char *str, int *checker_quotes, int *stalker)
{
	while (str[*stalker] && str[*stalker] != ' ' && str[*stalker] != '\t')
	{
		if (str[*stalker] == '\'' || str[*stalker] == '"')
		{
			*checker_quotes = 1;
			break ;
		}
		(*stalker)++;
	}
}
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

int	check_redirection_a(t_lexer *lexer)
{
	int	count_of_redirection;
	int	count_res;

	count_res = 0;
	count_of_redirection = 0;
	while (lexer)
	{
		if (*lexer->content == '>' && lexer->quotes != 0)
			count_of_redirection += redirection_counter(lexer->content);
		else if (*lexer->content != '>')
		{
			if (count_of_redirection > count_res)
				count_res = count_of_redirection;
			count_of_redirection = 0;
		}
		lexer = lexer->next;
	}
	if (count_of_redirection > count_res)
		count_res = count_of_redirection;
	return (count_res);
}

int	check_if_nofile_a(t_lexer *lexer)
{
	while (lexer)
	{
		if (*lexer->content == '>' && lexer->quotes != 0 && !lexer->next)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}

int	syntax_error_o_a(t_lexer *lexer)
{
	int	redirect;
	int	flage;

	redirect = check_redirection_a(lexer);
	flage = check_if_nofile_a(lexer);
	// printf("%d\n", edge_case_a(lexer));
	if (edge_case_a(lexer) == 1 && flage != 0 && redirect >= 2)
	{
		write(2, "minishell: syntax error near unexpected token `>'\n", 51);
		return (1);
	}
	if ((redirect == 1 || redirect == 2) && flage != 0)
	{
		write(2, "minishell: syntax error near unexpected token `newline'\n",
			57);
		return (1);
	}
	else if (redirect >= 4)
	{
		write(2, "minishell: syntax error near unexpected token `>>'\n", 51);
		return (1);
	}
	else if (redirect > 2)
	{
		write(2, "minishell: syntax error near unexpected token `>'\n", 51);
		return (1);
	}
	return (0);
}

int	check_redirection_h(t_lexer *lexer)
{
	int	count_of_redirection;
	int	count_res;

	count_res = 0;
	count_of_redirection = 0;
	while (lexer)
	{
		if (*lexer->content == '<' && lexer->quotes != 0)
			count_of_redirection += redirection_counter(lexer->content);
		else if (*lexer->content != '<')
		{
			if (count_of_redirection > count_res)
				count_res = count_of_redirection;
			count_of_redirection = 0;
		}
		lexer = lexer->next;
	}
	if (count_of_redirection > count_res)
		count_res = count_of_redirection;
	return (count_res);
}

int	check_if_nofile_h(t_lexer *lexer)
{
	while (lexer)
	{
		if (*lexer->content == '<' && lexer->quotes != 0 && !lexer->next)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}

int	syntax_error_i_h(t_lexer *lexer)
{
	int	redirect;
	int	flage;

	redirect = check_redirection_h(lexer);
	flage = check_if_nofile_h(lexer);
	if ((redirect >= 1 && redirect <= 3) && flage != 0)
	{
		write(2, "minishell: syntax error near unexpected token `newline'\n",
			57);
		return (1);
	}
	if (redirect >= 6)
	{
		write(2, "minishell: syntax error near unexpected token `<<<'\n", 53);
		return (1);
	}
	else if (redirect == 5)
	{
		write(2, "minishell: syntax error near unexpected token `<<'\n", 52);
		return (1);
	}
	else if (redirect == 4)
	{
		write(2, "minishell: syntax error near unexpected token `<'\n", 51);
		return (1);
	}
	return (0);
}

int	check_pipes(t_lexer *lexer)
{
	int	count_of_pipes;
	int	count_res;

	count_res = 0;
	count_of_pipes = 0;
	while (lexer)
	{
		if (*lexer->content == '|' && lexer->quotes != 0)
			count_of_pipes += redirection_counter(lexer->content);
		else if (*lexer->content != '|')
		{
			if (count_of_pipes > count_res)
				count_res = count_of_pipes;
			count_of_pipes = 0;
		}
		lexer = lexer->next;
	}
	if (count_of_pipes > count_res)
		count_res = count_of_pipes;
	return (count_res);
}

int	check_if_out(t_lexer *lexer)
{
	while (lexer)
	{
		if (*lexer->content == '|' && lexer->quotes != 0 && !lexer->next)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}

int	syntax_error_pipes(t_lexer *lexer)
{
	int	pipes;
	int	flage;

	pipes = check_pipes(lexer);
	flage = check_if_out(lexer);
	if ((lexer && !ft_strcmp(lexer->content, "|") && pipes == 1 && flage != 1)
		|| (pipes == 1 && flage != 0))
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
		return (1);
	}
	if (pipes > 1)
	{
		write(2, "minishell: syntax error near unexpected token `||'\n", 52);
		return (1);
	}
	return (0);
}

int	lexer(char *argv, t_lexer **lexer)
{
	*lexer = NULL;
	store_into_nodes(argv, lexer);
	if (syntax_error_o_a(*lexer) == 1)
		return (1);
	if (syntax_error_pipes(*lexer) == 1)
		return (1);
	if (syntax_error_i_h(*lexer) == 1)
		return (1);
	if (!lexer)
		return (0);
	// 	check_herdoc((*lexer)->next);
	// 	check_append((*lexer)->next);
	// 	check_pipe((*lexer)->next);
	return (0);
}
