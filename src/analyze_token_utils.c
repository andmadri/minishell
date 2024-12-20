/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_tkn_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marieke <marieke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 12:25:17 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/17 11:57:53 by marieke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	ft_quote_count(char *str, int length, char quote)
{
	int	i;
	int	num;

	i = 0;
	num = 0;
	while (i < length)
	{
		if (quote && str[i] == quote)
			num++;
		else if (!quote && (str[i] == '"' || str[i] == '\''))
			num++;
		i++;
	}
	return (num);
}

int	create_outfile(t_parse *info)
{
	int	token_length;

	token_length = info->next_token.length;
	if (info->cmd->outfile)
		free(info->cmd->outfile);
	if (info->cmd->out_fd != STDOUT_FILENO)
		close(info->cmd->out_fd);
	info->cmd->outfile = ft_create_str(info->next_token.start, token_length);
	if (!info->cmd->outfile)
		return (0);
	return (1);
}

static char	find_quote(char *str, int length)
{
	int		i;
	char	quote;

	quote = '\0';
	i = 0;
	while (str[i] && i < length)
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quote = str[i];
			break ;
		}
		i++;
	}
	return (quote);
}

char	*ft_create_str(char *str, int length)
{
	char	*ptr;
	char	quote;
	int		i;
	int		j;

	quote = find_quote(str, length);
	length -= ft_quote_count(str, length, quote);
	ptr = malloc(sizeof(char) * (length + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	j = 0;
	while (j < length)
	{
		if (quote && str[i] != quote)
			ptr[j++] = str[i++];
		else if (!quote && str[i] != '\'' && str[i] != '"')
			ptr[j++] = str[i++];
		else
			i++;
	}
	ptr[j] = '\0';
	return (ptr);
}

char	**expand_array(char **old_array, char *str)
{
	char	**new_array;
	int		old_array_length;
	int		i;

	if (!str)
		return (old_array);
	old_array_length = ft_arraylen(old_array);
	new_array = (char **)malloc((old_array_length + 2) * sizeof(char *));
	if (!new_array)
		return (free_dbl_array(&old_array), NULL);
	i = 0;
	while (old_array && old_array[i])
	{
		new_array[i] = old_array[i];
		i++;
	}
	new_array[i] = str;
	new_array[++i] = NULL;
	return (free(old_array), new_array);
}
