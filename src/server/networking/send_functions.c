#include		<sys/epoll.h>
#include		<dbg.h>

static void		poll_write(const int client_fd)
{
  /* Sepoll->event.data.fd = client_fd; */
  /* Sepoll->event.events = EPOLLIN | EPOLLRDHUP | EPOLLOUT; */
  /* epoll_ctl(Sepoll->efd, EPOLL_CTL_MOD, client_fd, &Sepoll->event); */
  return ;
}

static void		stop_poll_write(const int client_fd)
{
  /* Sepoll->event.data.fd = client_fd; */
  /* Sepoll->event.events = EPOLLIN | EPOLLRDHUP; */
  /* epoll_ctl(Sepoll->efd, EPOLL_CTL_MOD, client_fd, &Sepoll->event); */
  return ;
}

int			commit_response(const int client_fd, const char *response)
{
}

int			send_responses()
{
}
