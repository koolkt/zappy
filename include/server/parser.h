#ifndef			PARSER_H_
# define		PARSER_H_
# define		SLEEP_TIME 1000
# define		EQUAL 0
# define		INCOMPLETE -10
# define		NOT_EQUAL 1
# include		<server/c_buffer.h>
# include		<sys/epoll.h>
# define		NEW_CLIENT_CONNECTION ((t_player*)data)->fd == sg_listenfd

typedef enum e_p_state
  {
    UNKNOWN,
    AVANCE,
    DROITE,
    GAUCHE,
    VOIR,
    INCANTATION,
    INVENTAIRE,
    PPO,
    PLV,
    PIN,
    BCT,
    MCT,
    MSZ,
    SGT,
    SST,
    PREND,
    POSE,
    EXPULSE,
    FORK,
    BROADCAST,
    CONNECT_NBR,
    _P,
    _S,
    _B,
    _M,
    _IN,
    START,
  }		t_parse_state;

typedef int		(*t_read_event)(void *data);
typedef t_cbuf		*t_evqueue;
typedef struct		s_player
{
  const int		fd;
  t_read_event		read_func;
  int			p_id;
  const char		*ch;
  t_evqueue		eventq;
  int			karma;
  t_parse_state		state;
  const char		*incomp;
  int			timerfd;
}			t_player;

typedef struct		s_server
{
  const int		fd;
  t_read_event		read_func;
}			t_server;

typedef struct		s_cc
{
  t_read_event		rfunc;
  int			fd;
  t_epoll		*e;
}			t_cc;

int		parser(t_player *p);
int		get_command(void *data);
#endif
