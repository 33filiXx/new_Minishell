/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_10.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 05:00:31 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/13 05:00:47 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

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
