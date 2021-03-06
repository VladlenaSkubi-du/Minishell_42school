/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_completion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sschmele <sschmele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 18:24:29 by sschmele          #+#    #+#             */
/*   Updated: 2019/09/29 16:22:44 by sschmele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char				*non_printable_output(char c, char *cmd, size_t *all)
{
	extern char		**environ;
	size_t			j;
	size_t			i;
	char			**dirpath;
	char			*ptr;

	i = -1;
	ptr = NULL;
	j = 0;
	if (c == '\t' && (j = ft_strlen(cmd)) <= 2 && j > 0)
	{
		while (environ[j] && ft_strncmp(environ[j], "PATH=", 5) != 0)
			j++;
		if (environ[j] == NULL || environ[j][5] == '\0')
			return (cmd);
		dirpath = ft_strsplit(&environ[j][5], ':');
		while (dirpath[++i])
			if ((ptr = find_in_path_readline(dirpath[i], cmd)) != NULL)
				break ;
		if (dirpath != NULL && ptr != NULL)
			cmd = complete_cmd(cmd, ptr, all);
		ft_arrdel(dirpath);
	}
	return (cmd);
}

char				*find_in_path_readline(char *path, char *cmd)
{
	DIR				*path_dir;
	struct dirent	*entry;
	size_t			len;
	char			*ptr;

	len = 0;
	ptr = NULL;
	if ((path_dir = opendir(path)) == NULL)
	{
		closedir(path_dir);
		return (NULL);
	}
	len = ft_strlen(cmd);
	while ((entry = readdir(path_dir)))
	{
		if (ft_strncmp(entry->d_name, cmd, len) == 0)
		{
			ptr = ft_strdup(entry->d_name);
			closedir(path_dir);
			return (ptr);
		}
	}
	closedir(path_dir);
	return (NULL);
}

char				*complete_cmd(char *cmd, char *name, size_t *all)
{
	size_t			len;
	size_t			len_1;

	len = ft_strlen(name);
	len_1 = ft_strlen(cmd);
	all[7] = 0;
	if (all[0] < len)
		while (all[0] < len + 1)
		{
			cmd = ft_realloc(cmd, all[0], ft_strlen(cmd), all[0] * 2);
			all[0] *= 2;
		}
	len -= ft_strlen(cmd);
	ft_putstr(name + len_1);
	while (len--)
		all[2]++;
	all[3] = all[2];
	while (name[len_1 + all[7]])
	{
		cmd[len_1 + all[7]] = name[len_1 + all[7]];
		all[7]++;
	}
	free(name);
	return (cmd);
}
