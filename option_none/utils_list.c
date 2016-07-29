#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "my.h"
#include "ls.h"


t_flist    *mk_list_from_args(char **entry, int tabopt[5])
{
  t_flist   *entry_list;
  
  entry_list = 0;
  if (*entry == NULL)
    my_put_in_list_file(&entry_list, name_to_file("."));
  while (*entry)
    {
      if (tabopt[LITTLE_T]== 1)
	insert_by_date(&entry_list, name_to_file(*entry));
      else
	insert_by_name(&entry_list, name_to_file(*entry));
      entry++;
    }
  if (tabopt[LITTLE_R] == 1)
    my_rev_list_file(&entry_list);
  return (entry_list);
}

void	my_rev_list_file(t_flist **begin)
{
  t_flist        *next;
  t_flist        *prec;

  next = 0;
  prec = begin[0]->next;
  while (prec != 0)
    {
      begin[0]->next = next;
      next = *begin;
      *begin = prec;
      prec = begin[0]->next;
    }
  begin[0]->next = next;
}

void	insert_by_name(t_flist **begin, t_file file)
{
  t_flist *tmp;
  t_flist *current;
  t_flist *elem;
  
  tmp = NULL;
  current = *begin;
  if ((elem = malloc(sizeof(t_flist))) == NULL)
    exit(-1);
  elem->file = file;
  while (current && (my_strcmp(current->file.name, elem->file.name) < 0))
    {
      tmp = current;
      current = current->next;
    }
  elem->next = current;
  if (tmp != NULL)/*on ajoute notre element apres tmp et avant current */
    tmp->next = elem;
  else
    *begin = elem; /* Si tmp vaut null, alors on ajoute notre element en tete de liste */
}

void	insert_by_date(t_flist **begin, t_file file)
{
  t_flist *tmp;
  t_flist *current;
  t_flist *elem;

  tmp = NULL;
  current = *begin;
  if ((elem = malloc(sizeof(t_flist))) == NULL)
    exit(-1);
  elem->file = file;
  while (current && (cmp_date(current->file, elem->file) > 0))
    {
      tmp = current;
      current = current->next;
    }
  elem->next = current;
  if (tmp != NULL)/*on ajoute notre element apres tmp et avant current */
    tmp->next = elem;
  else
    *begin = elem; /* Si tmp vaut null, alors on ajoute notre element en tete de liste */
}

int	cmp_date(t_file file1,t_file file2)
{
  if ((file1.s.st_mtime - file2.s.st_mtime) > 0)/* *file 1 est superieur*/
    return (1);
  if ((file1.s.st_mtime - file2.s.st_mtime) < 0)/* file 2 est inferieur*/
    return (-1);
  /*if ((file1.s.st_mtime - file2.s.st_mtime) == 0)*/
  return (-(my_strcmp(file1.name, file2.name)));
}

int	my_flist_size(t_flist *begin)
{
  int	i;
    
  i = 0;
  while (begin != NULL)
    {
      begin = begin->next;
      i++;
    }
  return (i);
}

void                    my_put_in_list_file(t_flist **begin, t_file file)
{
  t_flist           *new_elem;

  new_elem = malloc(sizeof(*new_elem));
  new_elem->file = file;
  new_elem->next = *begin;
  *begin = new_elem;
}


void    my_show_list_file(t_flist *begin)
{
  while (begin != 0)
    {
      my_putstr(begin->file.name);
      my_putchar(' ');
       if (begin->file.error != NULL)
	 {
	   my_putstr(begin->file.error);
	 }
       else
	 {
	   
	   my_put_nbr(begin->file.s.st_mtime);
	 }
       my_putstr("\n");
       begin = begin->next;
    }
}


void    do_on_eq_in_flist(t_flist *begin, void (*f)(), void *data, int (*cmp)())
{
  t_flist       *tmp;

  tmp = begin;
  while (tmp)
    {
      if (cmp(data, tmp->file))
        {
          f(tmp->file);
        }
      tmp = tmp->next;
    }
}

void	do_on_eq_in_flist_long(t_flist *begin,  void (*f)(), void *data, int (*cmp)())
{
  t_flist       *tmp;

  tmp = begin;
  while (tmp)
    {
      if (cmp(data, tmp->file))
        {
          f(tmp->file, begin);
        }
      tmp = tmp->next;
    }
}

void	do_on_all_in_flist(t_flist *begin, void (*f)())
{
  t_flist	*tmp;

  tmp = begin;
  while (tmp)
    {
      f(tmp->file);
      tmp = tmp->next;
    }
}

void	do_on_all_in_flist_long(t_flist *begin, void (*f)())
{
  t_flist       *tmp;

  tmp = begin;
  while (tmp)
    {
      f(tmp->file, begin);
      tmp = tmp->next;
    }

}


t_flist  *rm_all_eq_in_flist(t_flist **begin, void *data_ref, int (*cmp)())
{
  t_flist                *tmp;
  t_flist                *prev;
  t_flist               *save;

  tmp = *begin;
  prev = tmp;
  while ((*begin != NULL) && (cmp(data_ref, (*begin)->file))) 
    {
      save = (*begin)->next;
      *begin = save;
    }
  while (tmp != 0)
    {
      if (cmp(data_ref, tmp->file))
        {
          prev->next = tmp->next;
	  if (tmp != 0)
	    free(tmp);
          prev = prev;
        }
      else
        prev = tmp;
      tmp = (*tmp).next;
    }
  return (*begin);
}



void	my_free_flist(t_flist **begin)
{
  t_flist *tmp;
  while(*begin)
    {
      tmp = (*begin)->next;
      free(*begin);
      *begin = tmp;
    }
}
t_file depile(t_flist **begin)
{
  t_file file;
  t_flist *tmp;
  
  /*if(!*begin)
    return (NULL);*/     
  tmp = (*begin)->next;
  file = (*begin)->file;
  free(*begin);
  *begin = tmp;             
  return (file);            
}
