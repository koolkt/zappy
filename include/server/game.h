#ifndef			GAME_H_
# define		GAME_H_
# include		<server/ev_queue.h>
# include		<stdlib.h>
# include		<stdint.h>

typedef struct		s_team
{
}			t_team;

typedef struct		s_map
{
  int			sx;
    int			sy;
}			t_map;

typedef struct		s_gplayer
{
  int			posx;
  int			posy;
  int			dir;
  uint64_t		inv;
  int			level;
  int			team;
}			t_gplayer;

int			game_new_player(char *team);
int			game_dead_player(t_event *e);
int			game_avance(t_event *e);
int			game_droite(t_event *e);
int			game_gauche(t_event *e);
t_map			get_map();
int			game_free_places(char *team);
int			get_map_size_x();
int			get_map_size_y();
int			init_map();
int			game_voir(t_event *e);
int			game_incantation(t_event *e);
int			game_inventaire(t_event *e);
int			game_prend(t_event *e);
int			game_pose(t_event *e);
int			game_expulse(t_event *e);
int			game_fork(t_event *e);
int			game_broadcast(t_event *e);

#endif
