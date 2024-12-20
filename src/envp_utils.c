/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marieke <marieke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:05:10 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/27 10:33:58 by marieke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	ft_arraylen(char **envp)
{
	int	i;

	i = 0;
	if (!envp || !*envp)
		return (i);
	while (envp[i])
		i++;
	return (i);
}

char	**copy_envp(char **envp)
{
	char	**copy_envp;
	int		i;
	int		num_env;

	copy_envp = NULL;
	i = 0;
	num_env = ft_arraylen(envp) + NULL_TERMINATOR;
	copy_envp = (char **)malloc(num_env * sizeof(char *));
	if (!copy_envp)
		return (NULL);
	while (envp[i])
	{
		copy_envp[i] = ft_strdup(envp[i]);
		if (!copy_envp)
		{
			free_dbl_array(&copy_envp);
			return (NULL);
		}
		i++;
	}
	copy_envp[i] = NULL;
	return (copy_envp);
}

int	replace_env(char *var_env, char *replace_str, t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(var_env, data->env[i], ft_strlen(var_env)) == 0)
		{
			free(data->env[i]);
			data->env[i] = ft_strjoin(var_env, replace_str);
			if (!data->env[i])
				return (error_memory_allocation(data, data->cmd_head));
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
