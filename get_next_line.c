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

#include "get_next_line.h"

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

static int	ft_create_line(int fd, char **line, char **other, char *buf)
{
	int		byte;
	char	*tmp;

	byte = read(fd, buf, BUFFER_SIZE);
	if (!*line && byte >= 0)
	{
		*line = ft_strdup("");
		if (!*line)
			return (-1);
	}
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
	free(buf);
	return (ft_check_byte(byte));
}

static int	ft_copy_other(int fd, char **line, char **other, char *buf)
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
		return (ft_create_line(fd, line, other, buf));
	}
	else
	{
		*next++ = '\0';
		*line = ft_strdup(*other);
		if (!*line)
			return (-1);
		*other = ft_strcpy(*other, next);
		free(buf);
		return (1);
	}
}

int	get_next_line(int fd, char **line)
{
	static char	*other;
	char		*buf;

	if (fd < 0 || BUFFER_SIZE <= 0 || !line)
		return (-1);
	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (-1);
	*line = NULL;
	if (!other)
		return (ft_create_line(fd, line, &other, buf));
	else
		return (ft_copy_other(fd, line, &other, buf));
}
