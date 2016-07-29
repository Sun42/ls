#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include "my.h"
#include "ls.h"

void	display_list_file(t_flist **begin, int tabopt[5]);
void    do_on_eq_in_flist(t_flist *begin, void (*f)(), void *data, int (*cmp)());
int     is_error(void *data_ref, t_file file);
void    display_error(t_file file);
t_flist  *rm_all_eq_in_flist(t_flist **begin, void *data_ref, int (*cmp)());
void	display_normal(t_file file);
int    is_type(void *data, t_file file);
void            display_dir(t_file rep, int tabopt[5], int tabslash[4]);
void    mk_tabslash(int tabslash[4], int nbrep);

void    display_list_from_args(t_flist ** begin, int tabopt[]);
void    display_list_from_rec(t_flist **begin, int tabopt[5]);


#define NB_ERR 0
#define NB_FIC 1
#define NB_REP 2
#define CUR_REP 3


int     type[] =
  {S_IFIFO, S_IFCHR, S_IFDIR, S_IFBLK, S_IFREG, S_IFLNK, S_IFSOCK};

int	main(int ac, char **av)
{
  int			tabopt[5];
  t_flist		*params_list;
  
  if (ac < 1)
    exit (-1);
  params_list  = check_args(ac, av, tabopt);

  if (my_flist_size(params_list) == 0)
    return (0);
  if (tabopt[LITTLE_D] == 1)
    display_list_as_file(&params_list, tabopt);
  else 
    display_list_from_args(&params_list, tabopt);
  return (0);
}

void	display_list_from_args(t_flist ** begin, int tabopt[])
{
  int		tabslash[4];
  t_file	rep;
  
  tabslash[NB_ERR] = my_flist_size(*begin);
  do_on_eq_in_flist(*begin, display_error, NULL, is_error);
  *begin = rm_all_eq_in_flist(begin, NULL, is_error);
  tabslash[NB_FIC] = my_flist_size(*begin);
  if (tabopt[LITTLE_L] == 1)
    {
      do_on_eq_in_flist_long(*begin, display_long, &type[2], is_not_type);
      *begin = rm_all_eq_in_flist(begin, &type[2], is_not_type);
    }
  else
    {
      do_on_eq_in_flist(*begin, display_normal, &type[4], is_type);
      *begin = rm_all_eq_in_flist(begin, &type[4], is_type);
    }
  mk_tabslash(tabslash, my_flist_size(*begin));
  while (my_flist_size(*begin) > 0)
    {
      rep = depile(begin);
      display_dir(rep, tabopt, tabslash);
      tabslash[CUR_REP]++;
    }
  my_free_flist(begin);
}

void	mk_tabslash(int	tabslash[4], int nbrep)
{
  tabslash[NB_REP] = nbrep;
  tabslash[NB_FIC] = tabslash[NB_FIC] - tabslash[NB_REP];
  tabslash[NB_ERR] = tabslash[NB_ERR] - (tabslash[NB_FIC] + tabslash[NB_REP]);
  tabslash[CUR_REP] = 1;
}

void		display_dir(t_file rep, int tabopt[5], int tabslash[4])
{
  t_flist	*my_list;

 
  if ((tabslash[CUR_REP] > 1) || (tabslash[NB_FIC] > 0))
    my_putchar('\n');
  if ((tabslash[NB_REP] > 1) || (tabslash[NB_FIC] > 0) || (tabslash[NB_ERR] > 0))
    {
      my_putstr(rep.name);
      my_putchar(':');
      my_putstr("\n");
    }
  my_list = dir_to_list(rep, tabopt);
  if (tabopt[LITTLE_L])
      {
	my_putstr("total ");
	my_put_nbr(recup_total(my_list));
	my_putchar('\n');
      }   
  if (tabopt[BIG_R] == 1)
    display_list_from_rec(&my_list, tabopt);
  else
    display_list_as_file(&my_list, tabopt);
}

void	display_list_from_rec(t_flist **begin, int tabopt[5])
{
  int           tabslash[4];
  t_file        rep;

  if (my_flist_size(*begin) == 0)
    return ;
  tabslash[NB_ERR] = my_flist_size(*begin);
  do_on_eq_in_flist(*begin, display_error, NULL, is_error);
  *begin = rm_all_eq_in_flist(begin, NULL, is_error);
  tabslash[NB_FIC] = my_flist_size(*begin);
  if (tabopt[LITTLE_L] == 1)
    do_on_all_in_flist_long(*begin, display_long);
  else
    do_on_all_in_flist(*begin, display_normal);
  *begin = rm_all_eq_in_flist(begin, &type[2], is_not_type);
  mk_tabslash(tabslash, my_flist_size(*begin));
  while (my_flist_size(*begin) > 0)
    {
      rep = depile(begin);
      display_dir(rep, tabopt, tabslash);
      tabslash[CUR_REP]++;
    }
  my_free_flist(begin);
}
