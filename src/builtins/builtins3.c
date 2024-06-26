/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzioual <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:53:43 by yzioual           #+#    #+#             */
/*   Updated: 2024/05/26 13:24:11 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool	_isbuiltin(t_arena *arena, char *input)
{
	char	**args;
	int		i;
	bool	is_builtin;

	i = 0;
	is_builtin = false;
	args = ft_split(arena, input, ' ');
	while (args[i])
	{
		args[i] = trim_quotes(arena, args[i]);
		i++;
	}
	if (ft_strcmp(args[0], "unset") == 0 || ft_strcmp(args[0], "pwd") == 0
		|| ft_strcmp(args[0], "exit") == 0 || ft_strcmp(args[0], "cd") == 0
		|| ft_strcmp(args[0], "echo") == 0 || ft_strcmp(args[0], "export") == 0
		|| ft_strcmp(args[0], "env") == 0)
		is_builtin = true;
	return (is_builtin);
}

int	_runbuiltins(t_stock *stock, char *input)
{
	if (ft_strncmp(trim_quotes(stock->arena, input), "pwd", 3) == 0)
		ft_pwd(stock->arena);
	else if (ft_strncmp("env", trim_quotes(stock->arena, input), 3) == 0)
		print_env(stock->env);
	else if (ft_strncmp(trim_quotes(stock->arena, input), "exit", 4) == 0)
		return (ft_exit(stock, input, stock->status));
	else if (ft_strncmp(trim_quotes(stock->arena, input), "cd", 2) == 0)
	{
		if (ft_strncmp(input, "cd..", 4) == 0)
			return (1);
		return (custom_cd(stock->arena, trim_quotes(stock->arena,
					input), stock->env));
	}
	else if (ft_strncmp(trim_quotes(stock->arena, input), "export",
			ft_strlen("export")) == 0)
		return (custom_export(stock, input));
	else if (ft_strncmp(trim_quotes(stock->arena, input), "unset",
			ft_strlen("unset")) == 0)
		custom_unset(stock, trim_quotes(stock->arena, input), stock->env);
	else if (ft_strncmp(trim_quotes(stock->arena, input), "echo",
			ft_strlen("echo")) == 0)
		ft_echo(stock->arena, trim_quotes(stock->arena, input));
	return (0);
}

char	*join_args(t_arena *arena, char **args)
{
	char	*input;
	size_t	total_length;
	size_t	i;

	total_length = 0;
	i = 0;
	while (args[i])
	{
		total_length += ft_strlen(args[i]);
		if (args[i + 1] != NULL)
			total_length++;
		i++;
	}
	input = arena_alloc(arena, total_length + 1);
	input[0] = '\0';
	while (*args)
	{
		ft_strcat(input, *args);
		if (*(args + 1) != NULL)
			ft_strcat(input, " ");
		args++;
	}
	return (input);
}

int	custom_cd(t_arena *arena, char *input, t_env *env)
{
	char	*cmd;
	char	*path;
	char	**args;
	int		i;

	i = 0;
	args = ft_split(arena, input, ' ');
	while (args[i])
		i++;
	if (i > 2)
	{
		ft_putstr_fd(" too many arguments\n", 2);
		return (1);
	}
	cmd = ft_strtok_2(input, " \t\n");
	path = ft_strtok_2(NULL, " \t\n");
	(void)cmd;
	return (ft_cd(arena, path, env));
}
