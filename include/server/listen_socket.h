#ifndef				DEFS_H_
# define			DEFS_H_
# include			<sys/types.h>
# include			<netinet/in.h>
# include			<arpa/inet.h>
# include			<errno.h>
# include			<sys/socket.h>
# include			<netdb.h>

typedef struct		s_listen
{
  struct sockaddr_in	s_in;
  socklen_t		size;
  int			port;
  int			socket_fd;
}			t_listen;
#endif
