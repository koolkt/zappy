#include		<server/defs.h>
#include		<dbg.h>
#include		<server/parser.h>
#include		<server/ev_queue.h>
#include		<server/game.h>
#include		<sys/timerfd.h>
#include		<time.h>
#include		<fcntl.h>
#define			CLIENT_FD (int)((t_cc*)((epoll_data_t*)data)->ptr)->fd
#define			R_FUNC (t_read_event)((t_cc*)((epoll_data_t*)data)->ptr)->rfunc

int			do_event(t_event *event, const int p_id, const int p_fd)
{
  static int	t = 1;
   
  debug("Executing event...");
  /* if (event->event == EV_AVANCE) */
  write(p_fd, "Avance\n", 7);
  printf("****************************** sent avance from server %i\n", t);
  t += 1;  
  return (EXIT_SUCCESS);
}

int			execute_event(void *data)
{
  t_event		*event;
  struct itimerspec	new_value;

  if (event = pop_event(((t_evq*)data)->eventq))
    do_event(event, ((t_evq*)data)->p_id, ((t_evq*)data)->p_fd);
  else
    {
      debug("Timer set but no events registered");
      new_value.it_value.tv_sec = 0;
      new_value.it_value.tv_nsec = 0;
      new_value.it_interval.tv_sec = 0;
      new_value.it_interval.tv_nsec = 0;
      timerfd_settime(((t_evq*)data)->eventq->timerfd,
		      TFD_TIMER_ABSTIME, &new_value, NULL);
    }
  debug("Event executed");
  return (EXIT_SUCCESS);
}

int			create_timer(t_cc* data, t_evqueue *evq, int p_id)
{
  struct itimerspec	new_value;
  t_evq			*tdata;
  t_evq			d = {-1, NULL, p_id, data->fd, NULL};
  int			timer_fd;

  tdata = malloc(sizeof(t_evq));
  check_mem(tdata);
  new_value.it_value.tv_sec = 0;
  new_value.it_value.tv_nsec = 0;
  new_value.it_interval.tv_sec = 0;
  new_value.it_interval.tv_nsec = 0;
  timer_fd = timerfd_create(CLOCK_MONOTONIC, O_NONBLOCK);
  *evq = create_evqueue(timer_fd);
  d.eventq = *evq;
  memcpy(tdata, &d , sizeof(t_evq));
  timerfd_settime(timer_fd, TFD_TIMER_ABSTIME, &new_value, NULL);
  tdata->timer_fd = timer_fd;
  tdata->read_func = execute_event;
  data->e->event.events = EPOLLIN | EPOLLRDHUP;
  data->e->event.data.ptr = tdata;
  epoll_ctl(data->e->efd, EPOLL_CTL_ADD, timer_fd, &data->e->event);
  return (timer_fd);
 error:
  return (-1);
}

int			init_client_data(const void* data, char *team)
{
  t_evqueue		evq;
  t_player		*player;
  t_player		p = {CLIENT_FD, R_FUNC, 0,
			     NULL, NULL, 1024, START, NULL, -1};
  
  player = malloc(sizeof(t_player));
  check_mem(player);
  p.p_id = game_new_player(team);
  p.timerfd = create_timer(((epoll_data_t*)data)->ptr, &evq, p.p_id);
  p.eventq = evq;
  memcpy(player, &p , sizeof(t_player));
  ((epoll_data_t*)data)->ptr = player;
  return (EXIT_SUCCESS);
 error:
  return (EXIT_FAILURE);
}
