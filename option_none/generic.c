#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include "my.h"
#include "ls.h"


void    *my_memset(void *str, int c, size_t len)
{
  register char *st;

  st = str;
  while (len-- > 0)
    *st++ = c;
  return str;
}

t_file name_to_file(char *name)
{
  t_file        file;
  struct stat s;

  my_memset(&file, 0, sizeof(file));
  my_memset(&s, 0, sizeof(s));
  file.name = name;
  if (lstat(name, &s) < 0)
    file.error = strerror(errno);
  file.s = s;
  return (file);
}
