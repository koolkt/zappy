#ifndef			C_BUFFER_H_
# define		C_BUFFER_H_

typedef struct		s_cbuf
{
  unsigned long int	size;
  unsigned int		head;
  unsigned int		tail;
  char			*data;
  int			timerfd;
  char			n_ev;
}			t_cbuf;

t_cbuf *new_circular_buffer(const unsigned int order);
void cbuf_free(t_cbuf *this);
int cbuf_is_empty(const t_cbuf *this);
int cbuf_write(t_cbuf *this, const void *data, const int size);
void *cbuf_peek(const t_cbuf *this);
void *cbuf_read(t_cbuf *this, const unsigned int size);
int cbuf_size(const t_cbuf *this);
int cbuf_usedspace(const t_cbuf *this);
int cbuf_freespace(const t_cbuf *this);
#endif
