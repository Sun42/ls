#include <stdlib.h>
#include "my.h"
#include "ls.h"

t_flist		*check_args(int ac, char **av, int tabopt[5])
{
  int		i;
  
  tabopt[0] = 0;
  tabopt[1] = 0;
  tabopt[2] = 0;
  tabopt[3] = 0;
  tabopt[4] = 0;
  i = 1;
  while ((i < ac) && (is_option(av[i], tabopt)))
    i = i + 1;
  return (mk_list_from_args((av + i), tabopt));
}

int	is_option(char *str, int tabopt[5])
{
  int	i;

  i = 0;
  if (str[0] != '-')
    return (0);
  i++;
  while (str[i])
    {
      if (!is_valid_option(str[i]))
        exit (-1);
      if (str[i] == 'R')
        tabopt[BIG_R] = 1;
      if (str[i] == 'r')
        tabopt[LITTLE_R] = 1;
      if (str[i] == 'l')
        tabopt[LITTLE_L] = 1;
      if (str[i] == 'd')
        tabopt[LITTLE_D] = 1;
      if (str[i] == 't')
        tabopt[LITTLE_T] = 1;
      i++;
    }
  return (1);
}

int	is_valid_option(char c)
{
  if (c == 'R')
    return (1);
  if (c == 'r')
    return (1);
  if (c == 'l')
    return (1);
  if (c == 'd')
    return (1);
  if (c == 't')
    return (1);
  my_putstr_error(OPT_ERR);
  my_putchar(c);
  my_putchar('\n');
  my_putstr_error(USAGE);
  my_putchar('\n');
  return (0);
}
