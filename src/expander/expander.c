/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzioual <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:39:43 by yzioual           #+#    #+#             */
/*   Updated: 2024/05/18 20:05:32 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_single_quotes(char c)
{
	if (c == '\'')
		return (true);
	return (false);
}

void	process_status_code(t_stock *stock, char **out_ptr)
{
    char	*var_value;
    
    var_value = ft_itoa(stock->arena, *(stock->status));
    ft_strcpy(*out_ptr, var_value);
    *out_ptr += ft_strlen(var_value);
}

void	process(const char **current, char **out_ptr, t_stock *stock)
{
	char var_name[128];
	char *var_value;
	char *start;
	t_env *env;
	int var_len = 0;

	start = *out_ptr;
	while (**current && (**current == '_' || isalnum(**current)))
		var_name[var_len++] = *(*current)++;
	var_name[var_len] = '\0';
	if (ft_strlen(var_name) == 0)
		env = find_env_var(&(stock->env), "__EXITSTATUS__");
	else
		env = find_env_var(&(stock->env), var_name);
	var_value = (env != NULL) ? env->value : var_name;
	if (var_value)
	{
		ft_strcpy(*out_ptr, var_value);
		*out_ptr += ft_strlen(var_value);
	}
	else
		*out_ptr = start;
}
/*
static void	traverse_and_process(const char *current, int inside_single_quotes, t_stock *stock, char *out_ptr)
{
	while (*current)
	{
		if (is_single_quotes(*current))
		{
			inside_single_quotes = !inside_single_quotes;
			*out_ptr++ = *current++;
		}
		else if (*current == '$' && !inside_single_quotes)
		{
			current++;
			if (*current == '?')
			{
				process_status_code(stock, &out_ptr);
				current++;
			}
			else
				process(&current, &out_ptr, stock);
		}	
		else
			*out_ptr++ = *current++;
	}
	*out_ptr = '\0';
}
*/
char	*expand_variables(t_stock *stock, char *input)
{
	const char	*current;
	char		*output;
	char		*out_ptr;
	int			inside_single_quotes;

	output = arena_alloc(stock->arena, ft_strlen(input) + 1024);
	current = input;
	out_ptr = output;
	inside_single_quotes = 0;
	//traverse_and_process(current, inside_single_quotes, stock, out_ptr);
	while (*current)
	{
		if (is_single_quotes(*current))
		{
			inside_single_quotes = !inside_single_quotes;
			*out_ptr++ = *current++;
		}
		else if (*current == '$' && !inside_single_quotes)
		{
			current++;
			if (*current == '?')
			{
				process_status_code(stock, &out_ptr);
				current++;
			}
			else
				process(&current, &out_ptr, stock);
		}	
		else
			*out_ptr++ = *current++;
	}
	*out_ptr = '\0';
	return (output);
}
