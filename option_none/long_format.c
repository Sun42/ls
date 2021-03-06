#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "my.h"
#include "ls.h"




int	types[] = 
  {
    S_IFIFO, 'p', S_IFCHR,'c', S_IFDIR,'d', S_IFBLK, 'b', S_IFREG, '-',
    S_IFLNK, 'l', S_IFSOCK, 's', 0, 0
  };

void    my_put_type(int mode)
{
  int   *p;

  for (p = &types[0];*(p + 1);p += 2)
    {
      if ((mode & S_IFMT) == *p)
        {
          my_putchar(*(p + 1));
          return ;
        }
    }
}



int     perm0[] = {1, S_IRUSR, 'r', '-'};
int     perm1[] = {1, S_IWUSR, 'w', '-'};
int     perm2[] = {3, S_ISUID|S_IXUSR, 's', S_IXUSR, 'x', S_ISUID, 'S', '-'};
int     perm3[] = {1, S_IRGRP, 'r', '-'};
int     perm4[] = {1, S_IWGRP, 'w', '-'};
int     perm5[] = {3, S_ISGID|S_IXGRP, 's', S_IXGRP, 'x', S_ISGID, 'S', '-'};
int     perm6[] = {1, S_IROTH, 'r', '-'};
int     perm7[] = {1, S_IWOTH, 'w', '-'};
int     perm8[] = {3, S_ISVTX|S_IXOTH, 't', S_IXOTH, 'x', S_ISVTX, 'T', '-'};
int     *perms[] =
  {perm0,perm1,perm2,perm3,perm4,perm5,perm6,perm7,perm8,0};
   
void    aff_perm(int mode)
{
  int   **defp;
  
  for (defp = &perms[0];*defp;defp++)
    {
      int       *def;
      int       n;

      def = *defp;
      for (n = *def++; n-- > 0;)
        if ((mode & *def) == *def)
          {
            def++;
            break ;
          }
        else
          def += 2;
      my_putchar(*def);
    }
}

char			*my_get_uid_name(uid_t uid)
{
  struct passwd		*s_pwd;
  char			*str_uid;
  
  if ((s_pwd = getpwuid(uid)) == NULL)
    {
      if ((str_uid = malloc(sizeof(*str_uid) * number_len(uid, 10) + 1)) == NULL)
	exit (-1);
      my_itoa(uid, str_uid);
      return (str_uid);/* a tester avec un uid pas connu*/
    }
  return (s_pwd->pw_name);
}

int	number_len(int n, int base)
{
  int	i;

  i = 0;
  while (n > 0)
    {
      n = n / base;
      i = i + 1;
    }
  if (i == 0)
    i++;
  return (i);
}
char		*my_get_grp_name(gid_t gid)
{
  struct group	*s_gr;
  char		*str_gid;
  
  if ((str_gid = malloc(sizeof(*str_gid) * number_len(gid, 10) + 1)) == NULL)
    exit (-1);
  if ((s_gr = getgrgid(gid)) == NULL)
    {
      my_itoa(gid, str_gid);
      return (str_gid);/* a tester avec un gid inconnu */
    }
  return (s_gr->gr_name);
}

char	*month[] = 
  {"Jan", "Feb", "Mar", "Apr", "May", "Jun", \
   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void			my_put_time(time_t timestamp)
{
  struct tm		*s_time;
 
  if ((s_time = localtime(&timestamp)) == NULL)
    return ;
  my_putstr(month[s_time->tm_mon]);
  my_putchar(' ');
  my_put_nbr_day(s_time->tm_mday);
  my_putchar(' ');
  if (more_six_month_diff(timestamp))
    {
      my_putchar(' ');
      my_put_nbr(s_time->tm_year + 1900);
    }
  else
    {
      my_put_nbr_hourmin(s_time->tm_hour);
      my_putchar(':');
      my_put_nbr_hourmin(s_time->tm_min);
    }
}

int	more_six_month_diff(time_t date)
{
  time_t	actual_time;

  actual_time = time(NULL);
  if (actual_time == (time_t)-1)
    {
      my_put_nbr_error(errno);
      exit (-1);
    }
  if ((actual_time - date) > (6 * 30 * 24 * 60  * 60)) 
    {
      return (1);
    }
  if ((actual_time - date) < (-15552000))
    {
      return (1);
    }
  return (0);
}

void	my_put_nbr_hourmin(int n)
{
  
  if (n < 10)
    my_putchar('0');
  my_put_nbr(n);
}

void	my_put_nbr_day(int n)
{
  if (n < 10)
    my_putchar(' ');
  my_put_nbr(n);
}
