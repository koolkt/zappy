#include			<server/z_epoll.h>
#include			<server/defs.h>
#include			<server/parser.h>
#include			<server/game.h>
#include			<sys/timerfd.h>

static t_epoll			*Sepoll;
static int			sg_listenfd;
static char			is_running = 1;

void		signal_callback(int signal)
{
  if (signal == SIGPIPE)
    log_err("Catched SIGPIPE");
  if (signal == SIGINT)
    {
      log_info("Shutting down...");
      is_running = 0;
    }
}

int delete_client(const void* data)
{
  printf("Hello from my delete client callback\n");
  return (EXIT_SUCCESS);
}

int			write_event(void *data)
{
  printf("Write event\n");
  return (0);
}

int			error_event(void *data)
{
  t_player*p;

  p = data;
  if (p->fd == -1)
    close(p->fd);
  epoll_ctl (Sepoll->efd, EPOLL_CTL_DEL, p->fd, &Sepoll->event);
  delete_client(data);
  if (data)
    free(data);
  return (EXIT_SUCCESS);
}

static int		read_event(void *data)
{
  int			free_places;
  int			br;
  char			buff[1024];
  char			str[16];
  int			infd;
  struct sockaddr_in	client_sin;
  socklen_t		client_sin_len;
  t_cc			d;

  br = 1;
  if (NEW_CLIENT_CONNECTION)
    {
      client_sin_len = sizeof(client_sin);
      infd = accept4(sg_listenfd, (struct sockaddr *)&client_sin,
		     &client_sin_len, SOCK_NONBLOCK);
      check(infd != -1, "add_new_client");
      write(infd, "BIENVENUE\n", 10);
      usleep(1000);
      br = read(infd, buff, 1023);
      check(br, "Client did not indicate team")
      buff[br] = '\0';
      free_places = game_free_places(buff);
      check(free_places, "No free places client kicked out");
      sprintf(str, "%d\n", free_places);
      write(infd, str, strlen(str));
      sprintf(str, "%d %d\n", get_map_size_x(), get_map_size_y());
      write(infd, str, strlen(str));
      Sepoll->event.events = EPOLLIN | EPOLLRDHUP;
      log_info("New client connected\n");
      d.fd = infd;
      d.rfunc = read_event;
      d.e = Sepoll;
      Sepoll->event.data.ptr = &d;
      init_client_data(&Sepoll->event.data, buff);
      epoll_ctl(Sepoll->efd, EPOLL_CTL_ADD, infd, &Sepoll->event);
    }
  else
    get_command(data);
  return (EXIT_SUCCESS);
 error:
  if (infd != -1 && br)
    write(infd, "ko\n", 3);
  return (EXIT_FAILURE);
}

int			add_server_socket(t_epoll *epoll)
{
  t_player		*player;
  t_player		p = {sg_listenfd, read_event, 0,
			     NULL, NULL, 1024, START, NULL};

  player = malloc(sizeof(t_player));
  check_mem(player);
  memcpy(player, &p , sizeof(t_player));
  epoll->event.data.ptr = player;
  epoll->event.events = EPOLLIN;
  epoll->s = epoll_ctl (epoll->efd, EPOLL_CTL_ADD, sg_listenfd, &epoll->event);
  return (EXIT_SUCCESS);
 error:
  return (EXIT_FAILURE);
}

char			init_epoll(const int listen_fd)
{
  int			*fd;
  t_epoll		*epoll;

  epoll = calloc (MAXEVENTS, sizeof(t_epoll));
  check_mem(epoll);
  epoll->efd = epoll_create1(0);
  check(epoll->efd != -1, "init_epoll");
  epoll->events = calloc(MAXEVENTS, sizeof(epoll->event));
  check_mem(epoll->events);
  sg_listenfd = listen_fd;
  add_server_socket(epoll);
  check(epoll->s != -1, "init_epoll"); 
  Sepoll = epoll;
  return (EXIT_SUCCESS);
 error:
  if (epoll->s != -1)
    close(epoll->s);
  if (epoll->efd != -1)
    close(epoll->efd);
  return (EXIT_FAILURE);
}

char		poll_for_client_events(void)
{
  int		i;
  int		client_fd;
  int		events;
  t_player	*data;

  check(sg_listenfd && Sepoll, "You need to call init_epoll"
	"before starting the loop");
  while(is_running)
    {
      events = epoll_wait (Sepoll->efd, Sepoll->events, MAXEVENTS, -1);
      for(i = 0; i < events; ++i)
	{
	  data = Sepoll->events[i].data.ptr;
	  if (FD_HAS_ERROR_OR_DISCONNECT)
	    error_event(Sepoll->events[i].data.ptr);
	  else if (FD_IS_READY_FOR_READ)
	    data->read_func(data);
	  else if (FD_IS_READY_FOR_WRITE)
	    write_event(Sepoll->events[i].data.ptr);
	  else
	    log_info("Client fd ready for something but didnt got catched"
		     "by ifs Event: %i FD: %i", events, client_fd);
	}
    }
  if (Sepoll && Sepoll->events)
    free(Sepoll->events);
  if (Sepoll)
    free(Sepoll);
  return (EXIT_SUCCESS);
 error:
  return (EXIT_FAILURE);
}
