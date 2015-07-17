#include		<server/game.h>

static t_map		map;

t_map			get_map()
{
  return (map);
}


int			game_free_places(char *team)
{
  return (1);
}

int			get_map_size_x()
{
  return (map.sx);
}

int			get_map_size_y()
{
  return (map.sy);
}

int			init_map()
{
}
