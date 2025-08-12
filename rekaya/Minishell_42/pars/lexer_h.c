/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_h.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:12:33 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/12 16:40:13 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../nrc/minishell.h"





t_lexer	*creat_node(char *content, e_tokens token, t_quotes quotes)
{
	t_lexer	*new_node;
	
	new_node = malloc(sizeof(t_lexer));
	if (!new_node)
		return (NULL);
	new_node->q = ft_calloc(sizeof(int) , 1);
	if (!new_node->q)
	{
		free_one_lexer(new_node);
		return (NULL);
	}
	new_node->lenght_double = ft_calloc(sizeof(int) , 1);
	if (!new_node->lenght_double)
	{
		free_one_lexer(new_node);
		return (NULL);
	}
	new_node->lenght_single = ft_calloc(sizeof(int), 1);
	if (!new_node->lenght_single)
	{
		free_one_lexer(new_node);
		return (NULL);
	}
	new_node->lenght_edge = ft_calloc(sizeof(int), 1);
	if (!new_node->lenght_edge)
	{
		free_one_lexer(new_node);
		return (NULL);
	}
	new_node->token = token;
	new_node->quotes = quotes;
   	new_node->q[0] = 1;
    // new_node->q[1] = 0;
	new_node->lenght_q = 0;
   	new_node->lenght_double[0] = 0;
	new_node->lenght_single[0] = 0;
	new_node->lenght_edge[0] = 0;
	new_node->lenght_normal = 0;
	new_node->just_one_case = 0;
    // new_node->lenght_double[1] = 0;
	new_node->content = ft_strdup(content);
	new_node->next = NULL;
	return (new_node);
}
void	insert_at_end(t_lexer **head, char *content, e_tokens token,
		t_quotes quotes)
{
	t_lexer	*new_node;
	t_lexer	*temp;

	new_node = creat_node(content, token, quotes);
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	temp = *head;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new_node;
}

void	ft_free_nodes(t_lexer *lexer)
{
	t_lexer	*tmp;

	while (lexer != NULL)
	{
		tmp = lexer->next;
		free(lexer->content);
		free(lexer);
		lexer = tmp;
	}
}
