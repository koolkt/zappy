#ifndef			Z_EPOLL_H_
# define		Z_EPOLL_H_
# define		MAXEVENTS 64
# define		FD_IS_READY_FOR_READ Sepoll->events[i].events & EPOLLIN
# define		FD_HAS_ERROR_OR_DISCONNECT Sepoll->events[i].events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)
# define		FD_IS_READY_FOR_WRITE Sepoll->events[i].events & EPOLLOUT
# define		_GNU_SOURCE 
# include		<stdbool.h>
# include		<sys/epoll.h>
# include		<dbg.h>
# include		<sys/types.h>
# include		<sys/socket.h>
# include		<arpa/inet.h>
# include		<signal.h>

typedef struct		s_epoll
{
  struct epoll_event	event;
  struct epoll_event	*events;
  int			s;
  int			efd;
}			t_epoll;

#endif
