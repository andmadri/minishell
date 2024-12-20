/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraasve <maraasve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 19:01:01 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/28 14:07:05 by maraasve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

char	**rearrange_env(char *ptr_unset, char **array)
{
	char	**cpy_env;
	int		i;
	int		j;

	i = 0;
	j = 0;
	cpy_env = (char **)malloc(((ft_arraylen(array) - 1) + 1) \
	* sizeof(char *));
	if (!cpy_env)
		return (NULL);
	while (array[i])
	{
		if (array[i] == ptr_unset)
		{
			free(array[i]);
			array[i++] = NULL;
			continue ;
		}
		cpy_env[j++] = array[i];
		array[i++] = NULL;
	}
	cpy_env[j] = NULL;
	free(array);
	array = cpy_env;
	return (cpy_env);
}

int	ft_unset(t_data *data, char **argv)
{
	int		i;
	int		j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (data->env[j])
		{
			if (ft_strncmp(argv[i], data->env[j], ft_env_len(argv[i])) == 0)
			{
				if (ft_strncmp("=", data->env[j] + ft_env_len(argv[i]), 1) == 0)
				{
					data->env = rearrange_env(data->env[j], data->env);
					if (!data->env)
						return (error_memory_allocation(data, data->cmd_head));
					break ;
				}
			}
			j++;
		}
		i++;
	}
	return (data->exit_status = EXIT_SUCCESS, EXIT_SUCCESS);
}
