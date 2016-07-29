#ifndef __MY_LS_H__
# define __MY_LS_H__

#define BIG_R 0
#define LITTLE_R 1
#define LITTLE_L 2
#define LITTLE_D 3
#define LITTLE_T 4
#define OPT_ERR "ls: illegal option -- " 
#define USAGE	"usage: ls [-Rdlrt] [file ...]"
#define BEGIN_ERROR "ls: "


#include <sys/stat.h>

typedef struct s_file
{
  char *name;
  char *error;
  struct stat s;
} t_file;

typedef struct          s_list_file
{
  t_file                file;
  struct s_list_file    *next;
} t_flist;

/*
** Args.h
*/
t_flist     *check_args(int ac, char **av, int tabopt[5]);/*Rempli le tableau d'options et retourne l'indice du prochain argument a utiliser  */
int     is_option(char *str, int tabopt[5]);
int     is_valid_option(char c);/*Verif  si un char fait parti des options */

/*
** utils_list.h
*/
t_flist    *mk_list_from_args(char **entry, int tabopt[5]);
void    my_put_in_list_file(t_flist **begin, t_file file);
void    my_show_list_file(t_flist *begin);
int     my_flist_size(t_flist *begin);
t_file	name_to_file(char *name);
int     cmp_date(t_file file1,t_file file2);
void    insert_by_date(t_flist **begin, t_file file);
void    insert_by_name(t_flist **begin, t_file file);
void    my_free_flist(t_flist **begin);

void	my_rev_list_file(t_flist **begin);
void    display_list_file(t_flist **begin, int tabopt[5]);
void    do_on_eq_in_flist(t_flist *begin, void (*f)(), void *data, int (*cmp)());
void	do_on_all_in_flist(t_flist *begin, void (*f)());
void    display_list_as_file(t_flist **begin, int tabopt[5]);
int     is_error(void *data_ref, t_file file);
void    display_error(t_file file);
t_flist  *rm_all_eq_in_flist(t_flist **begin, void *data_ref, int (*cmp)());
void    display_normal(t_file file);
int    is_type(void *data, t_file file);
t_file depile(t_flist **begin);


void    do_on_eq_in_flist_long(t_flist *begin,  void (*f)(), void *data, int (*cmp)());
void    do_on_all_in_flist_long(t_flist *begin, void (*f)());

/*
**long format
*/
void    my_put_time(time_t timestamp);
void    my_put_type(int mode);
void    aff_perm(int mode);
char    *my_get_uid_name(uid_t uid);
char    *my_get_grp_name(gid_t uid);
int     more_six_month_diff(time_t date);
void    my_put_nbr_hourmin(int n);
void    my_put_nbr_day(int n);
int     number_len(int n, int base);
void	make_col(int lencol, int lenword);


/*
**generique
*/
void    *my_memset (void *str, int c, size_t len);

/*
** Dir_to_list.h
*/
#ifdef HAVE_NOT_DIRENT_H
#include <sys/dir.h>
#else
#include <dirent.h>
#endif


t_flist    *dir_to_list(t_file rep, int tabopt[5]);
t_flist    *parse_rep(DIR *rep, int tabopt[5], char *pathname);
/*
**display.c
*/
void    display_long(t_file file, t_flist *flist);
void    display_into_dir(t_file rep, int tabopt[5]);
void    print_name(char *name);
int             recup_total(t_flist *begin);
void    print_link(char *filename);
int     len_col(t_file fic, char col);
int     maxlen_cols(t_flist *begin, char col);
int     is_not_type(void *data, t_file file);

#endif
