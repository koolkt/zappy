#include		<dbg.h>
#include		<server/defs.h>

void		signal_callback(int signal);

int		handle_signals()
{
  signal(SIGPIPE, signal_callback);
  signal(SIGINT, signal_callback);
}

int		my_new_client(const void *data);
int		my_delete_client(const void *data);
int		my_rcv(const void *data);

int		main(int argc, char **argv)
{
  int		err;
  int		port;
  int		listen_fd = -1;

  check_args(argc == 2 && (port = atoi(argv[1])) , "Usage %s host port", argv[0]);
  listen_fd = start_listen(port);
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
