/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_programs_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzioual <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 10:50:12 by yzioual           #+#    #+#             */
/*   Updated: 2024/05/28 00:20:31 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_delims	*get_delims(t_arena *arena, t_ast_node *root)
{
	t_delims	*delims;
	t_ast_node	*curr;
	t_ast_node	*prev;

	curr = root->right;
	prev = NULL;
	delims = arena_alloc(arena, sizeof(t_delims) * 3);
	while (curr->right != NULL)
	{
		prev = curr;
		curr = curr->right;
	}
	if (prev != NULL)
		delims->deli = ft_strdup(arena, prev->data);
	if (curr != NULL)
		delims->deli2 = ft_strdup(arena, curr->data);
	return (delims);
}

static char	*get_cmd(t_arena *arena, t_ast_node *root)
{
	if (root->left == NULL || root->left->data == NULL)
		return (NULL);
	return (ft_strdup(arena, root->left->data));
}

static char	**get_args(t_arena *arena, t_ast_node *root, char *cmd)
{
	t_ast_node	*temp;
	int			j;
	char		**args;

	if (cmd == NULL)
		return (NULL);
	args = arena_alloc(arena, sizeof(char *) * 100);
	j = 0;
	args[j++] = ft_strdup(arena, cmd);
	temp = root->left->right;
	while (temp != NULL)
	{
		args[j] = ft_strdup(arena, temp->data);
		j++;
		temp = temp->right;
	}
	args[j] = NULL;
	return (args);
}

static int	get_fd_out(t_arena *arena, t_ast_node *root)
{
	int			fd;
	int			fd_out;
	char		*file;
	t_ast_node	*temp;

	fd = 1;
	temp = root->left;
	while (temp->left != NULL)
	{
		file = ft_strdup(arena, temp->left->data);
		fd = open(file, O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
		if (fd == -1)
			return (-1);
		temp = temp->left;
	}
	fd_out = fd;
	return (fd_out);
}

t_program	*extract_program_heredoc(t_stock *stock, t_ast_node *root,
		int f_no_cmd)
{
	struct termios	old_termios;
	t_program		*program;
	t_delims		*delims;

	f_no_cmd = 0;
	tcgetattr(STDIN_FILENO, &old_termios);
	if (root->left == NULL)
		f_no_cmd = 1;
	program = arena_alloc(stock->arena, sizeof(t_program));
	delims = get_delims(stock->arena, root);
	if (process_heredoc(program, stock, &old_termios, delims) == -1)
		return (NULL);
	if (program->fd_in == -1)
		return (NULL);
	unlink("tmp.txt");
	if (f_no_cmd == 1)
		return (empty_program(program));
	program->cmd = get_cmd(stock->arena, root);
	program->fd_out = get_fd_out(stock->arena, root);
	stock->last_open_fd = program->fd_in;
	if (ft_strcmp(program->cmd, "echo") == 0)
		program->fd_in = 0;
	program->args = get_args(stock->arena, root, program->cmd);
	program->type = NODE_REDIRECTION_HEREDOC;
	return (program);
}
