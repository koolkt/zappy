#include		<server/defs.h>
#include		<dbg.h>
#include		<server/parser.h>

/*
 * Passing the client file descriptor as a pointer to void in event data. 
 * Then in the callback get the fd and change the poninter for a t_player pointer.
 */

int			get_command(void *data)
{
  int			br;
  char			buffer[1024];
  t_player		*player;

  br = 1;
  player = data;
  player->ch = buffer;
  if (!data || player->eventq->n_ev > 9)
    return (EXIT_SUCCESS);
  br = read(player->fd, buffer, 1023);
  check(br > 0, "Read err");
  buffer[br] = '\0';
  parser(data);
  check(player->karma > 1, "Player sent too many bad requests");
  return (EXIT_SUCCESS);
 error:
  if (!br || player->karma < 1)
    error_event(player);
  return (EXIT_FAILURE);
}
