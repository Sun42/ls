#include "my.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

void    aff_type(int mode);
void    aff_perm(int mode);

int	     main(int argc, char **argv)
{
  struct stat	s;
  int		mode;
  const char	*path;


  mode = 0700;
  
  if ((chmod(argv[1], mode)) == -1)
    {
      write(2, sterror(errno),);
    }
  
}


int     types[] = 
  {
    S_IFIFO,'p',S_IFCHR,'c',S_IFDIR,'d',S_IFBLK,'b',S_IFREG,'-',
    S_IFLNK,'l',S_IFSOCK,'s',0,0
  };

void    aff_type(int mode)
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
int     perm0[] = {1,S_IRUSR,'r','-'};
int     perm1[] = {1,S_IWUSR,'w','-'};
int     perm2[] = {3,S_ISUID|S_IXUSR,'s',S_IXUSR,'x',S_ISUID,'S','-'};
int     perm3[] = {1,S_IRGRP,'r','-'};
int     perm4[] = {1,S_IWGRP,'w','-'};
int     perm5[] = {3,S_ISGID|S_IXGRP,'s',S_IXGRP,'x',S_ISGID,'S','-'};
int     perm6[] = {1,S_IROTH,'r','-'};
int     perm7[] = {1,S_IWOTH,'w','-'};
int     perm8[] = {3,S_ISVTX|S_IXOTH,'t',S_IXOTH,'x',S_ISVTX,'T','-'};
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
      for (n = *def++;n-- > 0;)
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
