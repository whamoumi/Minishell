/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztouzri <ztouzri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 23:03:42 by ztouzri           #+#    #+#             */
/*   Updated: 2021/09/16 21:23:45 by ztouzri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_signal	g_signal;

void	heredocint(int sig)
{
	if (sig == SIGINT)
	{
		exit(1);
		rl_on_new_line();
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

t_bool	heredocisin(t_redir *current)
{
	while (current)
	{
		if (!ft_strcmp(current->redir, "<<"))
		{
			g_signal.heredocuse = true;
			return (true);
		}
		current = current->next;
	}
	return (false);
}

void	heredoc_fork2(int *fd, t_redir *current, char *line)
{
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, current->arg))
		{
			if (!heredocisin(current->next))
			{
				free(line);
				exit(0);
			}
			break ;
		}
		if (!heredocisin(current->next))
		{
			write(fd[1], line, ft_strlen(line));
			write(fd[1], "\n", 1);
		}
		free(line);
	}
}

void	heredoc_fork(int *fd, t_redir *current, char *line)
{
	close(fd[0]);
	signal(SIGINT, heredocint);
	signal(SIGQUIT, heredocint);
	while (current)
	{
		if (!ft_strcmp(current->redir, "<<"))
		{
			echo_control_seq(false);
			heredoc_fork2(fd, current, line);
			free(line);
		}
		current = current->next;
	}
	close(fd[1]);
	exit(0);
}

int	heredoc_non_pipe_command3(int tmpp, t_redir *current, char *line)
{
	int	fd[2];

	pipe(fd);
	g_signal.heredoc = heredocisin(current);
	if (!g_signal.heredoc)
		return (tmpp);
	g_signal.childpid = fork();
	if (!g_signal.childpid)
		heredoc_fork(fd, current, line);
	g_signal.heredoc = false;
	waitpid(g_signal.childpid, &g_signal.kill, 0);
	if (WEXITSTATUS(g_signal.kill))
		g_signal.kill = 1;
	else
		g_signal.kill = 0;
	close(fd[1]);
	tmpp = dup(fd[0]);
	close(fd[0]);
	return (tmpp);
}
