/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 05:05:47 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/13 05:19:19 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

int	*exit_stat(void)
{
	static int	stat;

	return (&stat);
}

void	execute_and_cleanup(int status, t_command **cmd, t_env *env)
{
	if (status == 0 && *cmd)
	{
		if ((*cmd)->pipe_in || (*cmd)->pipe_out)
			execute_pipeline(*cmd, env);
		else
			init_exc(*cmd, &env);
	}
	free_and_reset_cmd(cmd);
}

void	handle_lexer_ok(t_env *env, t_lexer **lexer_list,
		t_command **command)
{
	char	**env_char;
	int		parser_status;

	env_char = list_to_char_array(env);
	expand(env, *lexer_list);
	parser_status = parser(*lexer_list, command, env_char);
	free_split(env_char);
	execute_and_cleanup(parser_status, command, env);
	ft_free_lexer_list(*lexer_list);
	*lexer_list = NULL;
}

char	*check_quotes(char *s)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (s && s[i])
	{
		if (s[i] == '\'' && !dq)
			sq = !sq;
		if (s[i] == '\"' && !sq)
			dq = !dq;
		i++;
	}
	if (dq || sq)
		return (printf("minishell: Unclosed Quotes\n"), NULL);
	return (s);
}

char	*check_syntax(char *s)
{
	if (!check_quotes(s))
		return (NULL);
	return (s);
}
