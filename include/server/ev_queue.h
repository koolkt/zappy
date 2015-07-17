#ifndef			EV_QUEUE_H_
# define		EV_QUEUE_H_
# include		<server/c_buffer.h>

typedef int		(*t_read_event)(void *data);
typedef t_cbuf		*t_evqueue;

typedef enum		e_evtype
  {
    EV_UNKNOWN,
    EV_AVANCE,
    EV_DROITE,
    EV_GAUCHE,
    EV_VOIR,
    EV_INCANTATION,
    EV_INVENTAIRE,
    EV_PPO,
    EV_PLV,
    EV_PIN,
    EV_BCT,
    EV_MCT,
    EV_MSZ,
    EV_SGT,
    EV_SST,
    EV_PREND,
    EV_POSE,
    EV_EXPULSE,
    EV_FORK,
    EV_BROADCAST,
    EV_CONNECT_NBR,
  }			t_evtype;

typedef struct		s_evq
{
  int			timer_fd;
  t_read_event		read_func;
  const int		p_id;
  const int		p_fd;
  t_evqueue		eventq;
}			t_evq;

typedef struct		s_event
{
  t_evtype		event;
  int			p_id;
  char			data[256]; /* MAX 440 */
}			t_event;

t_evqueue		create_evqueue(int tfd);
int			is_evqueue_full(t_evqueue this);
int			push_event(t_evqueue this, t_evtype type, const char *data, int size);
t_event			*pop_event(t_evqueue this);
#endif
