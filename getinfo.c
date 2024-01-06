#include "shell.h"

/**
 * clear_info - initializes uninfo_t struct
 * @info: struct address
 */
void clear_info(uninfo_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_info - initializes uninfo_t struct
 * @info: struct address
 * @av: argument vector
 */
void set_info(uninfo_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = strtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		alias_replace(info);
		vars_replace(info);
	}
}

/**
 * info_free - frees uninfo_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void info_free(uninfo_t *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			list_free(&(info->env));
		if (info->history)
			list_free(&(info->history));
		if (info->alias)
			list_free(&(info->alias));
		ffree(info->environ);
			info->environ = NULL;
		mem_free((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}
