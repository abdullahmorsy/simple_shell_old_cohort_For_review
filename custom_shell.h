#ifndef CUSTOM_SHELL_H
#define CUSTOM_SHELL_H

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
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLUSH -1

/* for command chaining */
#define NORMAL_CMD 0
#define OR_CMD 1
#define AND_CMD 2
#define CHAINED_CMD 3

/* for convert_number() */
#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HISTORY_FILE ".custom_shell_history"
#define MAX_HISTORY 4096

extern char **environ;

/**
 *  * struct StringList - singly linked list
 *   * @num: the number field
 *    * @str: a string
 *     * @next: points to the next node
 *      */
typedef struct StringList
{
	    int num;
	        char *str;
		    struct StringList *next;
} StringList_t;

/**
 *  * struct CommandInfo - contains pseudo-arguments to pass into a function,
 *   *                    allowing a uniform prototype for a function pointer struct
 *    * @arg: a string generated from getline containing arguments
 *     * @argv: an array of strings generated from arg
 *      * @path: a string path for the current command
 *       * @argc: the argument count
 *        * @line_count: the error count
 *         * @err_num: the error code for exit()
 *          * @linecount_flag: if on count this line of input
 *           * @fname: the program filename
 *            * @env: linked list local copy of environ
 *             * @history: the history node
 *              * @alias: the alias node
 *               * @environ: custom modified copy of environ from LL env
 *                * @env_changed: on if environ was changed
 *                 * @status: the return status of the last exec'd command
 *                  * @cmd_buffer: address of pointer to cmd_buffer, on if chaining
 *                   * @cmd_buffer_type: CMD_type ||, &&, ;
 *                    * @read_fd: the fd from which to read line input
 *                     * @hist_count: the history line number count
 *                      */
typedef struct CommandInfo
{
	    char *arg;
	        char **argv;
		    char *path;
		        int argc;
			    unsigned int line_count;
			        int err_num;
				    int linecount_flag;
				        char *fname;
					    StringList_t *env;
					        StringList_t *history;
						    StringList_t *alias;
						        char **environ;
							    int env_changed;
							        int status;

								    char **cmd_buffer; /* pointer to cmd ; chain buffer, for memory management */
								        int cmd_buffer_type; /* CMD_type ||, &&, ; */
									    int read_fd;
									        int hist_count;
} CommandInfo_t;

#define COMMAND_INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	    0, 0, 0}

/**
 *  * struct BuiltinCommand - contains a builtin string and related function
 *   * @type: the builtin command flag
 *    * @func: the function
 *     */
typedef struct BuiltinCommand
{
	    char *type;
	        int (*func)(CommandInfo_t *);
} BuiltinCommand_t;

/* custom_shell.c */
int custom_shell(CommandInfo_t *, char **);
int find_builtin_command(CommandInfo_t *);
void find_command(CommandInfo_t *);
void fork_command(CommandInfo_t *);

/* path.c */
int is_command(CommandInfo_t *, char *);
char *duplicate_chars(char *, int, int);
char *find_command_path(CommandInfo_t *, char *, char *);

/* loop_custom_shell.c */
int loop_custom_shell(char **);

/* error_string_functions.c */
void error_puts(char *);
int error_putchar(char);
int put_fd(char c, int fd);
int puts_fd(char *str, int fd);

/* string_functions.c */
int custom_strlen(char *);
int custom_strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *custom_strcat(char *, char *);

/* string_functions2.c */
char *custom_strcpy(char *, char *);
char *custom_strdup(const char *);
void custom_puts(char *);
int custom_putchar(char);

/* string_functions3.c */
char *custom_strncpy(char *, char *, int);
char *custom_strncat(char *, char *, int);
char *custom_strchr(char *, char);

/* string_functions4.c */
char **split_string(char *, char *);
char **split_string2(char *, char);

/* memory_functions */
char *custom_memset(char *, char, unsigned int);
void free_strings(char **);
void *custom_realloc(void *, unsigned int, unsigned int);

/* memory_functions2.c */
int free_buffer(void **);

/* more_functions.c */
int interactive_mode(CommandInfo_t *);
int is_delimiter(char, char *);
int is_alpha(int);
int custom_atoi(char *);

/* more_functions2.c */
int error_atoi(char *);
void print_error(CommandInfo_t *, char *);
int print_decimal(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* builtin_emulators.c */
int custom_exit(CommandInfo_t *);
int custom_cd(CommandInfo_t *);
int custom_help(CommandInfo_t *);

/* builtin_emulators2.c */
int custom_history(CommandInfo_t *);
int custom_alias(CommandInfo_t *);

/* getline.c module */
ssize_t get_input(CommandInfo_t *);
int custom_getline(CommandInfo_t *, char **, size_t *);
void sigint_handler(int);

/* info.c module */
void clear_info(CommandInfo_t *);
void set_info(CommandInfo_t *, char **);
void free_info(CommandInfo_t *, int);

/* env.c module */
char *_getenv(CommandInfo_t *, const char *);
int custom_env(CommandInfo_t *);
int custom_setenv(CommandInfo_t *);
int custom_unsetenv(CommandInfo_t *);
int populate_env_list(CommandInfo_t *);

/* env2.c module */
char **get_custom_environ(CommandInfo_t *);
int custom_unsetenv(CommandInfo_t *, char *);
int custom_setenv(CommandInfo_t *, char *, char *);

/* file_io_functions.c */
char *get_history_file(CommandInfo_t *info);
int write_history(CommandInfo_t *info);
int read_history(CommandInfo_t *info);
int build_history_list(CommandInfo_t *info, char *buffer, int line_count);
int renumber_history(CommandInfo_t *info);

/* liststr.c module */
StringList_t *add_node(StringList_t **, const char *, int);
StringList_t *add_node_end(StringList_t **, const char *, int);
size_t print_string_list(const StringList_t *);
int delete_node_at_index(StringList_t **, unsigned int);
void free_string_list(StringList_t **);

/* liststr2.c module */
size_t list_length(const StringList_t *);
char **list_to_string_array(StringList_t *);
size_t print_list(const StringList_t *);
StringList_t *node_starts_with(StringList_t *, char *, char);
ssize_t get_node_index(StringList_t *, StringList_t *);

/* chain.c */
int is_chain(CommandInfo_t *, char
int is_chain(CommandInfo_t *, char *, size_t *);
void check_chain(CommandInfo_t *, char *, size_t *, size_t, size_t);
int replace_alias(CommandInfo_t *);
int replace_vars(CommandInfo_t *);
int replace_string(char **, char *);

#endif
