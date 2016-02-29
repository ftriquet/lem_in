#include <lem_in.h>
#include <libft.h>

t_room	*get_best_ngh(t_room *room, t_map *map)
{
	t_room	*r;
	size_t	i;
	int		min;
	t_room	*it;

	i = 0;
	min = INT_MAX;
	while (i < room->neighbours.size)
	{
		it = ft_list_get_at(&room->neighbours, i);
		if (it == map->end)
			return (it);
		if (min > it->weight && !it->used && it != map->start)
		{
			min = it->weight;
			r = it;
		}
		++i;
	}
	r->used = 1;
	return (r);
}

void	cut_links(t_room *room, t_map *map)
{
	size_t	i;
	t_room	*best;
	t_room	*tmp;

	best = get_best_ngh(room, map);
	i = 0;
	while (i < room->neighbours.size)
	{
		tmp = ft_list_get_at(&room->neighbours, i);
		if (tmp != best)
		{
			ft_list_remove(&room->neighbours, tmp, 0, &ft_room_cmp);
			--i;
		}
		i++;
	}
}
