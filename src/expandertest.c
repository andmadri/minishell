/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandertest.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marieke <marieke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 22:10:03 by marieke           #+#    #+#             */
/*   Updated: 2024/06/27 10:34:10 by marieke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

char	*copy_realloc(char *new_str, int capacity, char c)
{
	char	*temp;
	int		i;

	capacity += 1;
	i = 0;
	temp = (char *)malloc(capacity * sizeof(char));
	if (!temp)
		return (free(new_str), NULL);
	while (new_str && new_str[i])
	{
		temp[i] = new_str[i];
		i++;
	}
	temp[i] = c;
	temp[i + 1] = '\0';
	return (free(new_str), temp);
}

int	expand_exit_status(t_data *data, char **new_str)
{
	char	*exit_code;
	char	*tmp;

	exit_code = ft_itoa(data->exit_status);
	if (!exit_code)
		return (0);
	tmp = *new_str;
	*new_str = ft_strjoin(tmp, exit_code);
	free(tmp);
	free(exit_code);
	if (!*new_str)
		return (error_memory_allocation(data, data->cmd_head), 0);
	return (1);
}

int	expand_env(t_data *data, char **new_str, char *input, int *len)
{
	int		i;
	char	*env_var;
	char	*tmp;

	i = 0;
	while (input[i] && (ft_isalpha(input[i]) || input[i] == '_'))
		i++;
	env_var = find_env(input, i, data->env);
	if (env_var)
	{
		tmp = *new_str;
		*new_str = ft_strjoin(tmp, env_var);
		free (tmp);
		if (!*new_str)
			return (error_memory_allocation(data, data->cmd_head), 0);
	}
	*len += i + 1;
	return (1);
}

int	ft_expand(t_data *data, char **new_str, char *input, int *len)
{
	int	i;

	i = 1;
	if (ft_isdigit(input[i]))
		*len += 2;
	else if (input[i] == '?')
	{
		if (!expand_exit_status(data, new_str))
			return (0);
		*len += 2;
	}
	else if (ft_isalpha(input[i]) || input[i] == '_')
	{
		if (!expand_env(data, new_str, &input[i], len))
			return (0);
	}
	else
	{
		*new_str = copy_realloc(*new_str, ft_strlen(*new_str) + 1, input[0]);
		if (!new_str)
			return (error_memory_allocation(data, data->cmd_head), 0);
		*len += 1;
	}
	return (1);
}

char	*expander_test(t_data *data, int i)
{
	char	*new_str;
	bool	squotes;
	bool	dquotes;

	squotes = false;
	dquotes = false;
	new_str = NULL;
	i = 0;
	while (i < (int)ft_strlen(data->input) && data->input[i])
	{
		if (data->input[i] == '"' && !squotes)
			dquotes = !dquotes;
		else if (data->input[i] == '\'' && !dquotes)
			squotes = !squotes;
		if (data->input[i] == '$' && !squotes)
		{
			if (!ft_expand(data, &new_str, &data->input[i], &i))
				return (free(new_str), NULL);
		}
		new_str = copy_realloc(new_str, ft_strlen(new_str) + 1, data->input[i]);
		i++;
		if (!new_str)
			return (error_memory_allocation(data, data->cmd_head), NULL);
	}
	return (new_str);
}
