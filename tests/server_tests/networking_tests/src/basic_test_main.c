#include		<dbg.h>
#include		<server/defs.h>
static char		is_running = 1;

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
  init_epoll(&listen_fd);
  register_callback()->new_client = my_new_client;
  register_callback()->delete_client = my_delete_client;
  register_callback()->rcv_event = my_rcv;
  while(is_running)
    {
      err = poll_for_client_events();
      check(err != 1, "main_loop %i", err);
      /* debug("Tick"); */
    }
  check(close(listen_fd) != -1, "Closing server fd");
  clean_up_epoll();
  return (EXIT_SUCCESS);
 error:
  if (listen_fd != -1)
    close(listen_fd);
    clean_up_epoll();
  return (EXIT_FAILURE);
}
