#include		<server/c_buffer.h>
#include		<server/ev_queue.h>
#include		<string.h>
#include		<dbg.h>
#include		<stdlib.h>
#include		<sys/timerfd.h>
#include		<time.h>

static int		T = 100;

int			set_timer(int timer_fd, float t)
{
  int			i;
  long			n;
  struct itimerspec	new_value;
  struct timespec	now;

  clock_gettime(CLOCK_MONOTONIC, &now);
  i = t < 0 ? 0 : t;
  n = (t - (float)i) * 10000000;
  debug("Sec %i nsec %u", i, n);
  new_value.it_value.tv_sec = i ? now.tv_sec + i : 0;
  new_value.it_value.tv_nsec = n ? now.tv_nsec + n : 0;
  new_value.it_interval.tv_sec = 0;
  new_value.it_interval.tv_nsec = 0;
  timerfd_settime(timer_fd, TFD_TIMER_ABSTIME, &new_value, NULL);
}

t_evqueue		create_evqueue(int tfd)
{
  t_evqueue		new;

  new = new_circular_buffer(12);
  check_mem(new);
  new->timerfd = tfd;
  return (new);
 error:
  return (NULL);
}

int			is_evqueue_full(t_evqueue this)
{
  return (this->n_ev < 10 ? 0 : 1);
}

int			push_event(t_evqueue this, t_evtype type, const char *data, int size)
{
  t_event		*next;
  t_event		*event;

  if(is_evqueue_full(this))
    return(0);
  event = malloc(sizeof(t_event));
  check_mem(event);
  event->event = type;
  if (size)
    memcpy(event->data, data, size);
  cbuf_write(this, event, sizeof(t_event));
  if (!this->n_ev)
    {
      if (type != EV_INCANTATION &&
	  type != EV_FORK)
	set_timer(this->timerfd, 7.0f/T);
      else if (type == EV_INCANTATION)
	set_timer(this->timerfd, 300.0f/T);
      else if (type == EV_FORK)
	set_timer(this->timerfd, 42.0f/T);
      else
	set_timer(this->timerfd, 0); 
      debug("Set timer at push");
    }
  this->n_ev += 1;
  return (1);
 error:
  return (0);
}

t_event			*pop_event(t_evqueue this)
{
  t_event		*next;
  t_event		*current;

  check(this && cbuf_size(this), "Pop ev of empty");
  debug("n of events in queue at pop %i", this->n_ev);
  current = cbuf_read(this, sizeof(t_event));
  this->n_ev -= 1;
  next = cbuf_peek(this);
  if (next && next->event != EV_INCANTATION &&
      next->event != EV_FORK)
    {
      debug("curr %p next %p", current, next);
      set_timer(this->timerfd, 7.0f/T);
    }
  else if(next && next->event == EV_INCANTATION)
    set_timer(this->timerfd, 300.0f/T);
  else if(next && next->event == EV_FORK)
    {
      debug("pop event FORK %i %s", next->event, next->data);
      set_timer(this->timerfd, 42.0f/T);
    }
  else if (!next)
    {
      set_timer(this->timerfd, 0);
    }
  return (current);
 error:
  return (NULL);
}
