#include		<server/game.h>
#define			NORTH 1
#define			SOUTH 2
#define			EAST 3
#define			WEST 4

static t_team		teams[1024];
static t_gplayer	players[1024];

int			game_new_player(char *team)
{
  static int		p = 1;

  ++p;
  players[p].posx = 0;
  players[p].posy = 0;
  players[p].dir = NORTH;
  players[p].inv = 0;
  players[p].level = 1;
  players[p].team = 0;
  return (p);
}

int			game_dead_player(t_event *e)
{
  return (EXIT_SUCCESS);
}

int			game_avance(t_event *e)
{
  return (EXIT_SUCCESS);
}

int			game_droite(t_event *e)
{
  return (EXIT_SUCCESS);
}

int			game_gauche(t_event *e)
{
  return (EXIT_SUCCESS);
}
