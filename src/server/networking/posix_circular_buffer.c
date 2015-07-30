#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>
#include	<sys/mman.h>
#include	<unistd.h>
#include	<server/c_buffer.h>
#include	<dbg.h>

#ifndef		MAP_ANONYMOUS
#  define	MAP_ANONYMOUS MAP_ANON
#endif

static char	_create_buffer_mirror(t_cbuf* cb)
{
  char		path[] = "/tmp/cb-XXXXXX";
  int		fd;
  int		status;
  void		*address;

  fd = mkstemp(path);
  check(fd != -1, "Create_mirror path");
  status = unlink(path);
  check(!status, "_Create mirror");
  status = ftruncate(fd, cb->size);
  cb->data = mmap(NULL, cb->size << 1, PROT_NONE,
		  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  check(cb->data != MAP_FAILED, "Create mirror mmap1");
  address = mmap(cb->data, cb->size, PROT_READ | PROT_WRITE,
		 MAP_FIXED | MAP_SHARED, fd, 0);
  check(address == cb->data, "Create mirror mmap2");
  address = mmap(cb->data + cb->size, cb->size, PROT_READ | PROT_WRITE,
		 MAP_FIXED | MAP_SHARED, fd, 0);
  check(address == (cb->data + cb->size), "Create mirror mmap3");
  status = close(fd);
  check(!status, "Create mirror close");
  return (EXIT_SUCCESS);
 error:
  return (EXIT_FAILURE);
}

t_cbuf		*new_circular_buffer(const unsigned int order)
{
  char		s;
  t_cbuf	*this;

  this = malloc(sizeof(t_cbuf));
  check_mem(this);
  this->size = 1UL << order;
  this->head = 0;
  this->tail = 0;
  s = _create_buffer_mirror(this);
  check(s == EXIT_SUCCESS, "new_circular_buffer");
  return (this);
 error:
  return(NULL);
}

void		cbuf_free(t_cbuf *this)
{
  if (this)
    {
      munmap(this->data, this->size << 1);
      free(this);
    }
  return ;
}

int		cbuf_write(t_cbuf *this, const void *data, const int size)
{
  int		written;

  check_mem(this);
  check(cbuf_freespace(this) >= size && data && size > 0, "cbuf_write");
  written = cbuf_freespace(this);
  written = size < written ? size : written;
  memcpy(this->data + this->tail, data, written);
  this->tail += written;
  if (this->size < this->tail)
    this->tail %= this->size;
  return (written);
 error:
  return(0);
}

void		*cbuf_peek(const t_cbuf *this)
{
  if (!this || !this->n_ev)
    return (NULL);
  return (this->data + this->head);
}

void		*cbuf_read(t_cbuf *this, const unsigned int size)
{
  void		*end;

  if (!this || !this->n_ev)
    return (NULL);
  end = this->data + this->head;
  this->head += size;
  return (end);
}

int		cbuf_size(const t_cbuf *this)
{
  if (this)
    return (this->size);
  else
    return (0);
}

int		cbuf_usedspace(const t_cbuf *this)
{
  if (!this)
    return (-1);
  if (this->head <= this->tail)
    return (this->tail - this->head);
  else
    return( this->size - (this->head - this->tail));
}

int		cbuf_freespace(const t_cbuf *this)
{
  if (!this)
    return (-1);
  return (this->size - cbuf_usedspace(this));
}
