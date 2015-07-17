#ifndef				DEFS_H_
# define			DEFS_H_
# define			UNUSED(x) (void)(x)
# include			<unistd.h>
# include			<stdio.h>
# include			<stdlib.h>
# include			<time.h>
# include			<errno.h>
# include			<signal.h>
# include			<server/z_epoll.h>

int				write_event(void *data);
int				error_event(void *data);
int				start_listen(int p);
char				poll_for_client_events(void);
char				init_epoll(const int listen_fd);
int				init_client_data(const void* data, char *team);
#endif
