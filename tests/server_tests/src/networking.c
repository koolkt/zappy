#include	<server/parser.h>
#include	<server_tests/networking_tests.h>

int		initialise_suite()
{
  printf("Initiaizing suite...\n");
  return(0);
}

int		cleanup_suite()
{
  printf("\nCleaning suite...\n");
  return(0);
}

  /*   const int		fd; */
  /* t_read_event		read_func; */
  /* int			p_id; */
  /* const char		*ch; */
  /* t_evqueue		eventq; */
  /* int			karma; */
  /* t_parse_state		state; */
  /* const char		*incomp; */
  /* int			timerfd; */

    /* 0 UNKNOWN, */
    /* 1 AVANCE, */
    /* 2 DROITE, */
    /* 3 GAUCHE, */
    /* 4 VOIR, */
    /* 5 INCANTATION, */
    /* 6 INVENTAIRE, */
    /* 7 PPO, */
    /* 8 PLV, */
    /* 9 PIN, */
    /* 10 BCT, */
    /* 11 MCT, */
    /* 12 MSZ, */
    /* 13 SGT, */
    /* 14 SST, */
    /* 15 PREND, */
    /* 16 POSE, */
    /* 17 EXPULSE, */
    /* 18 FORK, */
    /* 19 BROADCAST, */
    /* 20 CONNECT_NBR, */
    /* 21 _P, */
    /* 22 _S, */
    /* 23 _B, */
    /* 24 _M, */
    /* 25 _IN, */
    /* 26 START, */

void		test_naive_parser()
{
  t_player	p = {0};
  char		*state[27] = {" "};
  char		*result[12] = {" "};
  char		*expected[12] = {" "};
  t_parse_state	n;
  int		i;

  p.ch = "avance\ndcddcdcdroite\n";
  state[1] = "avance";
  state[2] = "droite";
  expected[0] = "avance";
  expected[1] = "droite";
  p.state = START;
  i = 0;
  while ((n = parser(&p)) != -1)
    {
      result[i] = state[n];
      i++;
    }
  while(i--)
    CU_ASSERT_STRING_EQUAL(result[i], expected[i])
  return;
}

/* void		test_event_loop() */
/* { */
/*   CU_ASSERT_EQUAL(, 0); */
/* } */

