#include <lem_in.h>
#include <stdlib.h>
#include <libft.h>

t_map				parse_file(int fd)
{
	char			*line;
	t_map			map;
	t_parsing_msg	flag;

	ft_bzero(&map, sizeof(map));
	ft_list_init(&(map.rooms), NULL);
	get_nb_ants(fd, &map);
	while (get_next_line(fd, &line) == 1)
	{
		add_line(&map, line);
		flag = handle_line(fd, &line, &map);
		if (flag == INVALID_LINE || flag == END_PARSING)
			break ;
	}
	if (!map.start)
		exit_error("");
	if (!map.end)
		exit_error("");
	return (map);
}

void				add_line(t_map *map, char *line)
{
	t_line	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return ;
	new->data = line;
	new->next = NULL;
	if (map->lines)
	{
		new->next = map->lines;
		map->lines = new;
	}
	else
		map->lines = new;
}

void				get_nb_ants(int fd, t_map *map)
{
	char			*line;

	while (42)
	{
		if (get_next_line(fd, &line) != 1)
			exit_error("");
		add_line(map, line);
		if (line[0] == '#' && line[1] == '\0')
			continue ;
		if (is_int(line))
		{
			map->ants = ft_atoi(line);
			break ;
		}
		else
			exit_error("");
	}
}

t_parsing_msg		handle_line(int fd, char **line, t_map *map)
{
	static int		room_flag = 1;
	t_parsing_msg	msg;

	if ((*line)[0] == '\0')
		return (END_PARSING);
	if ((*line)[0] == '#')
		return (handle_sharp(fd, line, map));
	if (!ft_strchr(*line, ' '))
		room_flag = 0;
	if (room_flag == 1)
		msg = handle_room_line(line, map);
	else
		msg = handle_link_line(line, map);
	return (msg);
}

t_parsing_msg		handle_room_line(char **line, t_map *map)
{
	t_room			*new_room;

	new_room = is_valid_room(map, *line);
	if (new_room)
	{
		add_room(map, new_room, 0);
		return (OK);
	}
	return (INVALID_LINE);
}


t_parsing_msg		handle_sharp(int fd, char **line, t_map *map)
{
	if (!(*line))
		return (INVALID_LINE);
	if ((*line)[0] == '#' && (*line)[1] != '#')
		return (COMMENT);
	else if ((*line)[0] == '#' && (*line)[1] == '#')
		return (handle_special_line(fd, line, map));
	return (OK);
}

t_parsing_msg		handle_special_line(int fd, char **line, t_map *map)
{
	int				flag;
	t_room			*new_room;

	if (!ft_strequ(*line, "##start") && !ft_strequ(*line, "##end"))
		return (INVALID_LINE);
	flag = ft_strequ(*line, "##start") - ft_strequ(*line, "##end");
	if (get_next_line(fd, line) != 1)
		return (INVALID_LINE);
	add_line(map, *line);
	new_room = is_valid_room(map, *line);
	if (new_room)
		add_room(map, new_room, flag);
	else
		return (INVALID_LINE);
	return (OK);
}

int					add_room(t_map *map, t_room *room, int flag)
{
	if (flag == 1 && map->start)
		exit_error("");
	if (flag == -1 && map->end)
		exit_error("");
	ft_list_add_back(&map->rooms, room, sizeof(t_room));
	if (flag == 1)
		map->start = room;
	if (flag == -1)
		map->end = room;
	return (0);
}
