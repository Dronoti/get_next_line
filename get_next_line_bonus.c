/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkael <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 07:54:18 by bkael             #+#    #+#             */
/*   Updated: 2021/05/21 17:33:14 by bkael            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static int	ft_create_other(char **other, char *buf)
{
	*other = ft_strchr(buf, '\n');
	if (*other)
	{
		**other = '\0';
		(*other)++;
		*other = ft_strdup(*other);
		if (!*other)
			return (-1);
	}
	return (0);
}

static int	ft_check_byte(int byte)
{
	if (byte == 0)
		return (0);
	if (byte < 0)
		return (-1);
	return (1);
}

static int	ft_create_line(int fd, char **line, char **other)
{
	int		byte;
	char	buf[BUFFER_SIZE + 1];
	char	*tmp;

	byte = read(fd, buf, BUFFER_SIZE);
	if (byte < 0)
		return (-1);
	if (!*line)
		*line = ft_strdup("");
	if (!*line)
		return (-1);
	while (!*other && byte > 0)
	{
		buf[byte] = '\0';
		if (ft_create_other(other, buf) == -1)
			return (-1);
		tmp = *line;
		*line = ft_strjoin(*line, buf);
		free(tmp);
		if (!*line)
			return (-1);
		if (!*other)
			byte = read(fd, buf, BUFFER_SIZE);
	}
	return (ft_check_byte(byte));
}

static int	ft_copy_other(int fd, char **line, char **other)
{
	char	*next;

	next = ft_strchr(*other, '\n');
	if (!next)
	{
		*line = ft_strdup(*other);
		if (!*line)
			return (-1);
		free(*other);
		*other = NULL;
		return (ft_create_line(fd, line, other));
	}
	else
	{
		*next++ = '\0';
		*line = ft_strdup(*other);
		if (!*line)
			return (-1);
		*other = ft_strcpy(*other, next);
		return (1);
	}
}

int	get_next_line(int fd, char **line)
{
	static char	*other[1024];

	if (fd < 0 || BUFFER_SIZE <= 0 || !line)
		return (-1);
	if (*line)
		free(*line);
	*line = NULL;
	if (!other[fd])
		return (ft_create_line(fd, line, &other[fd]));
	else
		return (ft_copy_other(fd, line, &other[fd]));
}
