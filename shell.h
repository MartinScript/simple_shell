#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct list_str
{
	int num;
	char *str;
	struct liststr *next;
} lists_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	lists_t *env;
	lists_t *history;
	lists_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} uninfo_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(uninfo_t *);
} builtin_tab;

/* atoi.c */
int interactive(uninfo_t *);
int _is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* builtin.c */
int _myexit(uninfo_t *);
int _mycd(uninfo_t *);
int _myhelp(uninfo_t *);

/* builtin1.c */
int _myhistory(uninfo_t *);
int _myalias(uninfo_t *);

/* environ.c */
char *_getenv(uninfo_t *, const char *);
int _myenv(uninfo_t *);
int _mysetenv(uninfo_t *);
int _myunsetenv(uninfo_t *);
int populate_env_list(uninfo_t *);

/* errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* errors1.c */
int _erratoi(char *);
void print_error(uninfo_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void delete_comments(char *);

/* exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* getenv.c */
char **get_environ(uninfo_t *);
int _unsetenv(uninfo_t *, char *);
int _setenv(uninfo_t *, char *, char *);

/* getinfo.c */
void clear_info(uninfo_t *);
void set_info(uninfo_t *, char **);
void info_free(uninfo_t *, int);

/*getline.c */
ssize_t get_input(uninfo_t *);
int _getline(uninfo_t *, char **, size_t *);
void sigintHandler(int);

/* history.c */
char *get_history_file(uninfo_t *info);
int write_history(uninfo_t *info);
int read_history(uninfo_t *info);
int build_history_list(uninfo_t *info, char *buf, int linecount);
int renumber_history(uninfo_t *info);

/* lists.c */
lists_t *add_node(lists_t **, const char *, int);
lists_t *add_node_end(lists_t **, const char *, int);
size_t print_list_str(const lists_t *);
int delete_node_at_index(lists_t **, unsigned int);
void list_free(lists_t **);

/* lists1.c */
size_t list_len(const lists_t *);
char **lists_to_strings(lists_t *);
size_t print_list(const lists_t *);
lists_t *node_starts_with(lists_t *, char *, char);
ssize_t node_index(lists_t *, lists_t *);

/* memory.c */
int mem_free(void **);

/* parser.c */
int is_cmd(uninfo_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(uninfo_t *, char *, char *);

/* realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* shell_loop.c */
int hsh(uninfo_t *, char **);
int builtins_finder(uninfo_t *);
void cmd_search(uninfo_t *);
void cmd_fork(uninfo_t *);

/* string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* vars.c */
int is_chain(uninfo_t *, char *, size_t *);
void chain_check(uninfo_t *, char *, size_t *, size_t, size_t);
int alias_replace(uninfo_t *);
int vars_replace(uninfo_t *);
int string_replace(char **, char *);

/* loophsh.c */
int loophsh(char **);

#endif