#include "my.h"
#include "ls.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


#define MAX(a,b) ((a) > (b)) ? (a) : (b);
 

/*Mode -d */
void	display_list_as_file(t_flist **begin, int tabopt[5])
{
  if (my_flist_size(*begin) == 0)
    return ;
  do_on_eq_in_flist(*begin, display_error, NULL, is_error);
  *begin = rm_all_eq_in_flist(begin, NULL, is_error);
  if (tabopt[LITTLE_L] == 1)/*Mode -l */
    do_on_all_in_flist_long(*begin, display_long);
  else
    do_on_all_in_flist(*begin, display_normal);
  my_free_flist(begin);
}

/*Mode normal */


/*
void	display_into_dir(t_file rep, int tabopt[5])
{
  t_flist	*my_list;

  my_putchar('\n');
  my_putstr(rep.name);
  my_putchar(':');
  my_putchar('\n');
  my_list = dir_to_list(rep, tabopt);
  if (tabopt[LITTLE_L])
    {
      my_putstr("total ");
      my_put_nbr(recup_total(my_list));
      my_putchar('\n');
    }
  if (tabopt[BIG_R] == 1)
    display_list_file(&my_list, tabopt);
  else
    display_list_as_file(&my_list, tabopt);
}
*/
int		recup_total(t_flist *begin)
{
  int	cpt;

  cpt = 0;
  while (begin)
    {
      cpt = cpt + begin->file.s.st_blocks;
      begin = begin->next;
    }
  /*  cpt = cpt / 2;
  if ((cpt % 2) != 0)
  cpt++;*/
  return (cpt);
}

void	display_error(t_file file)
{
  my_putstr("ls: ");
  print_name(file.name);
  my_putstr(": ");
  my_putstr(file.error);
  my_putstr("\n");
}

void	display_normal(t_file file)
{
  print_name(file.name);
  my_putstr("\n");
}

void	print_name(char *name)
{
  int	i;
  int	j;
  int	flag;

  i = 0;
  flag = 0;
  my_revstr(name);
  while ((flag == 0) && (name[i] != '\0'))
    {
      if (name[i] == '/')
	flag = 1;
      i = i + 1;
    }
  my_revstr(name);
  if (flag == 0)
    my_putstr(name);
  else
    {
      j = ((my_strlen(name) - i) + 1);
      while (name[j] != '\0')
	my_putchar(name[j++]);
    }
}
 
void	display_long(t_file file, t_flist *flist)
{
  my_put_type(file.s.st_mode);
  aff_perm(file.s.st_mode);
  my_putchar(' ');
  make_col(maxlen_cols(flist, 'n'), number_len(file.s.st_nlink, 10)) ;
  my_put_nbr(file.s.st_nlink);
  my_putchar(' ');
  my_putstr(my_get_uid_name(file.s.st_uid));
  make_col(maxlen_cols(flist, 'u'), my_strlen((my_get_uid_name(file.s.st_uid))));
  my_putchar(' ');
  my_putstr(my_get_grp_name(file.s.st_gid));
  make_col(maxlen_cols(flist, 'g'), my_strlen((my_get_grp_name(file.s.st_gid))));
  make_col(maxlen_cols(flist, 's'), number_len(file.s.st_size, 10));
  my_put_nbr(file.s.st_size);
  make_col(maxlen_cols(flist, 't'), number_len(file.s.st_mtime, 10));
  my_put_time(file.s.st_mtime);
  my_putchar(' ');
  print_name(file.name);
  if ((file.s.st_mode & S_IFMT) == S_IFLNK)
    print_link(file.name);
  my_putchar('\n');
}

void	make_col(int lencol, int lenword)
{
  int	i;
  
  i = 0;
  my_putchar(' ');
  while (i < (lencol -lenword))
    {
      my_putchar(' ');
      i++;
    }	
}
/*
**Retourne la plus grande occurence d'une colonne dans une liste chainee
*/
int	maxlen_cols(t_flist *begin, char col)
{
  int	cpt;
  
  cpt = 0;
  while (begin != 0)
    {
      cpt = MAX(cpt, len_col(begin->file, col));
      begin = begin->next;
    }
  return (cpt);
}

int	len_col(t_file fic, char col)
{
  if (col == 's')
    return (number_len(fic.s.st_size, 10));
  if (col == 't')
    return (number_len(fic.s.st_mtime, 10));
  if (col == 'g')
    return (my_strlen(my_get_grp_name(fic.s.st_gid)));
  if (col == 'u')
    return (my_strlen(my_get_uid_name(fic.s.st_uid)));
  if (col == 'n')
    return (number_len(fic.s.st_nlink, 10));
  else 
    return (0);
}

void	print_link(char *filename)
{
  char	buf[512];
  int	bufsiz;
  int	i;
  
  bufsiz = 512;
  i = 0;
  while (i < bufsiz)
    {
      buf[i] = '\0';
      i++;
    }
  my_putstr(" -> ");
  if (readlink(filename, buf, bufsiz) == -1)
    {
      my_put_nbr_error(errno);
      return ;
    }
  my_putstr(buf);
}

int     is_type(void *data, t_file file)
{
  int	*p;

  p = data;
  if ((file.s.st_mode & S_IFMT) == *p)
    {
      return (1);
    }
  return (0);
}

int	is_not_type(void *data, t_file file)
{
  int	*p;

  p = data;
  if ((file.s.st_mode & S_IFMT) != *p)
    return (1);
  return (0);
}

int     is_error(void *data, t_file file)
{
  if (file.error != data)
    return (1);
  return (0);
}
