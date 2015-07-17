#include		<server/defs.h>
#include		<dbg.h>
#include		<server/parser.h>
#include		<server/ev_queue.h>

static const char	*commands[40] =
  {
    [AVANCE] = "vance\n",
    [DROITE] = "roite\n",
    [GAUCHE] = "auche\n",
    [VOIR] = "oir\n",
    [_IN] = "n",
    [INCANTATION] = "antation\n",
    [INVENTAIRE] = "entaire\n",
    [PREND] = "end ",
    [POSE] = "se ",
    [EXPULSE] = "xpulse\n",
    [FORK] = "ork\n",
    [BROADCAST] = "roadcast ",
    [CONNECT_NBR] = "onnect_nbr\n",
    [PPO] = "o ",
    [PLV] = "v ",
    [PIN] = "n ",
    [BCT] = "t ",
    [MCT] = "t\n",
    [MSZ] = "z\n",
    [SGT] = "t\n",
    [SST] = "t "
  };

static t_parse_state	*state[40] = {NULL};
static t_parse_state	start[255] = {UNKNOWN};
static t_parse_state	in[255] = {UNKNOWN};
static t_parse_state	p[255] = {UNKNOWN};
static t_parse_state	b[255] = {UNKNOWN};
static t_parse_state	m[255] = {UNKNOWN};
static t_parse_state	s[255] = {UNKNOWN};

static int		my_strcmp(const char **str1, const char **str2)
{
  if (!**str2)
    { 
      return (EQUAL);
    }
  if (!**str1)
    {
      return (INCOMPLETE);
    }
  if (**str1 == **str2)
    {
      ++(*str1);
      ++(*str2);
      return (my_strcmp(str1, str2));
    }
  return (NOT_EQUAL);
}

/*
 *  Parse buffer and compare it against a control string if buffer ends and control 
 *  string hasnt it means it might be incomplete, so we save the current position 
 *  in control string as a pointer in p->incomp.
 *
*/

static void		profile_command(t_player *p, const char *control_str)
{
  const char		*sv;
  const char		*pt;
  int			r;

  sv = p->ch;
  if (!p->incomp)
    p->ch += 1;
  pt = p->incomp ? p->incomp : control_str;
  r = my_strcmp(&p->ch, &pt);
  if (r == EQUAL && p->state != _IN)
    {
      push_event(p->eventq, p->state, "hello\n", 6);
    }
  else if (r == INCOMPLETE)
    {
      p->incomp = pt;
      return ;
    }
  if (p->incomp && r != EQUAL)
    p->ch = sv;
  p->incomp = NULL;
  if (p->state == _IN && r == EQUAL)
    return;
  p->state = START;
  p->karma -= 1;
  return ;
}

static void		init_letter_to_state()
{
  state[UNKNOWN] = start;
  state[START] = start;
  state[_IN] = in;
  state[_P] = p;
  state[_B] = b;
  state[_M] = m;
  state[_S] = s;
  /* state[PREND] = stones;  PARSE THIS TO TEST INCOMPLETE COMAND*/
  /* state[POSE] = stones; */
  start['a'] = AVANCE;
  start['d'] = DROITE;
  start['g'] = GAUCHE;
  start['v'] = VOIR;
  start['i'] = _IN;
  start['p'] = _P;
  start['e'] = EXPULSE;
  start['f'] = FORK;
  start['b'] = _B;
  start['s'] = _S; 
  start['c'] = CONNECT_NBR;
  in['v'] = INVENTAIRE;
  in['c'] = INCANTATION;
  p['r'] = PREND;
  p['o'] = POSE;
  p['p'] = PPO;
  p['l'] = PLV;
  p['i'] = PIN;
  b['r'] = BROADCAST;
  b['c'] = BCT;
  m['c'] = MCT;
  m['s'] = MSZ;
  s['s'] = SGT;
  s['s'] = SST;
  /* stones[] = ; */
}

int		parser(t_player *p)
{
  if (!state[START])
    init_letter_to_state();
  if (!p || !p->ch || !*p->ch)
    return (0);  
  if (state[p->state])
    p->state = state[p->state][*p->ch];
  if (p->state && p->state != _P &&
      p->state != _M && p->state != _B &&
      p->state != _S)
    {
      profile_command(p, commands[p->state]);
    }
  else
    {
      p->ch++;
      /* Bug! here the karma 
      will be demenished not only with unknown commands 
      but also with ambigupus letters. FIX THAT*/
      p->karma -= 1;
    }
  return (parser(p));
}
