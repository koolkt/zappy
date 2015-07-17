#include		<dbg.h>
#include		<server/defs.h>
#include		<ctype.h>

void		signal_callback(int signal);

int		handle_signals()
{
  signal(SIGPIPE, signal_callback);
  signal(SIGINT, signal_callback);
}

typedef struct	s_opts
{
  int		port;
  int		x;
  int		y;
  int		max_clients;
  int		t;
  char		*teams;
}		t_opts;

int		init_opts(int argc, char **argv, t_opts *op)
{
  int		c;

  if (argc <= 1)
    return (EXIT_FAILURE);
  while ((c = getopt (argc, argv, "p:x:y:c:t:n:")) != -1)
    switch (c)
      {
      case 'p':
	op->port = atoi(optarg);
	break;
      case 'x':
	op->x = atoi(optarg);
	break;
      case 'y':
	op->y = atoi(optarg);
	break;
      case 'c':
	op->max_clients = atoi(optarg);
	break;
      case 't':
	op->t = atoi(optarg);
	break;
      case 'n':
	op->teams = optarg;
	break;
      case '?':
	if (optopt == 'c' || optopt == 'p' || optopt == 'x' ||
	    optopt == 'y' || optopt == 't' || optopt == 'n')
	  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	else if (isprint (optopt))
	  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	else
	  fprintf (stderr,
		   "Unknown option character `\\x%x'.\n",
		   optopt);
	return (EXIT_SUCCESS);
      default:
	return (EXIT_FAILURE);
      }
  return (EXIT_FAILURE);
}

int		main(int argc, char **argv)
{
  t_opts	op = {0};;
  int		err;
  int		port;
  int		listen_fd = -1;

  if (!init_opts(argc, argv, &op))
    return (0);
  if ( !op.port || !op.x || !op.y || !op.max_clients || !op.teams)
    {
      printf("usage: ./zappy-server-linux [[[-p port] -p port] ...] "
	     "[-x world_x] [-y world_y] [-c max_clients] [-t speed] -n "
	     "team_name_1 team_name_2 ...\n");
      return (0);
    }
  printf("Teams %s\n", op.teams);
  listen_fd = start_listen(op.port);
  check(listen_fd != -1, "start_listen failed");
  handle_signals();
  init_epoll(listen_fd);
  poll_for_client_events();
  check(close(listen_fd) != -1, "Closing server fd");
  return (EXIT_SUCCESS);
 error:
  if (listen_fd != -1)
    close(listen_fd);
  return (EXIT_FAILURE);
}
