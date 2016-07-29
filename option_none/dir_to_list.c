#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef HAVE_NOT_DIRENT_H
# include <sys/dir.h>
#else
# include <dirent.h>
#endif
#include "my.h"
#include "ls.h"

char *mk_full_name(char *path, char *name);

t_flist		*dir_to_list(t_file file, int tabopt[5])
{
  DIR	*rep;
  t_flist	*my_list;

  my_list = 0;
  if ((rep = opendir(file.name)) == NULL)
    {
      my_putstr_error(BEGIN_ERROR);
      my_putstr_error(file.name);
      my_putstr(": ");
      my_put_nbr_error(errno);
    }
  else
    {
      my_list =  parse_rep(rep, tabopt, file.name);
      closedir(rep);
    }
  return (my_list);
}

t_flist		*parse_rep(DIR *rep, int tabopt[5], char *pathname)
{
  t_flist	*my_list;
  char		*final_name;

#ifdef HAVE_NOT_DIRENT_H
  struct direct		*s_rep;
#else
  struct dirent		*s_rep;
#endif
 
  my_list = 0;
  final_name = NULL;
  while ((s_rep = readdir(rep)) != NULL)
    {
      final_name = mk_full_name(pathname, s_rep->d_name);
      if ((my_strncmp(s_rep->d_name, ".", 1) != 0) && (my_strcmp(s_rep->d_name, "..") != 0))
	{
	  if (tabopt[LITTLE_T] == 1)
	    insert_by_date(&my_list, name_to_file(final_name));
	  else
	    insert_by_name(&my_list, name_to_file(final_name));
	}
    }
  free(s_rep);
  if (tabopt[LITTLE_R] == 1)
    my_rev_list_file(&my_list);
    return (my_list);
}


char *mk_full_name(char *path, char *name)
{
  char *final_name;
  int	len;
    
  len = (my_strlen(path) + my_strlen(name) + 2);
  if ((final_name = malloc(sizeof(*final_name) * len )) == NULL)
    exit (-1);
  my_strcpy(final_name, path);
  my_strcat(final_name, "/");
  my_strcat(final_name, name);
  return (final_name);
}
