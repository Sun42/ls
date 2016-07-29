#include "my.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void	my_put_time(time_t timestamp);
void	my_put_type(int mode);
void    aff_perm(int mode);
void    my_put_nbr_error(int numerror);
void    my_putstr_error(char *error);
char    *my_get_uid_name(uid_t uid);
char	*my_get_grp_name(gid_t uid);
int	more_six_month_diff(time_t date);
void    my_put_nbr_hourmin(int n);
void    my_put_nbr_day(int n);



int     main(int argc, char **argv)
{
  struct stat   s;

  if (argc != 2)
    {
      my_putstr_error("usage: stat file\n");
      exit(1);
    }
  if (stat(argv[1], &s) < 0)
    {
      my_put_nbr_error(errno);
      exit(-1);
    }
  my_put_type(s.st_mode);
  aff_perm(s.st_mode);
  my_putchar(' ');
  my_putchar(' ');
  my_put_nbr(s.st_nlink);
  my_putchar(' ');
  my_putstr(my_get_uid_name(s.st_uid));
  my_putchar(' ');
  my_putchar(' ');
  my_putstr(my_get_grp_name(s.st_gid));
  my_putchar(' ');
  my_putchar(' ');
  my_put_nbr(s.st_size);
  my_putchar(' ');
  my_put_time(s.st_mtime);
  my_putchar(' ');
  my_putstr(argv[1]);
  my_putchar('\n');
}


int	types[] = 
  {
    S_IFIFO, 'p', S_IFCHR,'c', S_IFDIR,'d', S_IFBLK, 'b', S_IFREG, '-',
    S_IFLNK, 'l', S_IFSOCK, 's', 0, 0
  };

void	my_put_type(int mode)
{
  int	*p;
  
  p = &types[0];
  while (*(p + 1) != 0)
    {
      if ((mode & S_IFMT) == *p)
	{
	  my_putchar(*(p + 1));
	  return ;
	}
      p = p + 2;
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

void	my_put_nbr_error(int numerror)
{
  char	*error;
  
  error = strerror(numerror);
  write(2, error, my_strlen(error));
}

void	my_putstr_error(char *error)
{
  write(2, error, my_strlen(error));
}

char			*my_get_uid_name(uid_t uid)
{
  struct passwd		*s_pwd;
  char			*str_uid;
  
  if ((s_pwd = getpwuid(uid)) == NULL)
    {
      my_itoa(uid, str_uid);
      return (str_uid);/* a tester avec un uid pas connu*/
    }
  return (s_pwd->pw_name);
}

char		*my_get_grp_name(gid_t gid)
{
  struct group	*s_gr;
  char		*str_gid;
  
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
      exit(-1);
    }
  if (difftime(actual_time, date) > (6 * 30 * 24 * 60  * 60)) 
    {
      //my_putstr("Fichier inferieur de 6 mois");
      return (1);
    }
  if (difftime(actual_time, date) < (-15552000))
    {
      //my_putstr("FIchier superieur de 6 mois");
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
