nclude "custom_shell.h"

/**
 *  * custom_exit - exits the shell
 *   * @info: Structure containing potential arguments. Used to maintain
 *    * constant function prototype.
 *     * Return: exits with a given exit status
 *      * (0) if info->argv[0] != "exit"
 *       */
int custom_exit(CommandInfo_t *info)
{
	    int exit_check;

	        if (info->argv[1])  /* If there is an exit argument */
			    {
				            exit_check = custom_atoi(info->argv[1]);
					            if (exit_check == -1)
							            {
									                info->status = 2;
											            print_error(info, "Illegal number: ");
												                error_puts(info->argv[1]);
														            error_putchar('\n');
															                return (1);
																	        }
						            info->err_num = custom_atoi(info->argv[1]);
							            return (-2);
								        }
		    info->err_num = -1;
		        return (-2);
}

/**
 *  * custom_cd - changes the current directory of the process
 *   * @info: Structure containing potential arguments. Used to maintain
 *    * constant function prototype.
 *     * Return: Always 0
 *      */
int custom_cd(CommandInfo_t *info)
{
	    char *s, *dir, buffer[1024];
	        int chdir_ret;

		    s = getcwd(buffer, 1024);
		        if (!s)
				        custom_puts("TODO: >>getcwd failure emsg here<<\n");
			    if (!info->argv[1])
				        {
						        dir = _getenv(info, "HOME=");
							        if (!dir)
									            chdir_ret = /* TODO: what should this be? */
											                    chdir((dir = _getenv(info, "PWD=")) ? dir : "/");
								        else
										            chdir_ret = chdir(dir);
									    }
			        else if (custom_strcmp(info->argv[1], "-") == 0)
					    {
						            if (!_getenv(info, "OLDPWD="))
								            {
										                custom_puts(s);
												            custom_putchar('\n');
													                return (1);
															        }
							            custom_puts(_getenv(info, "OLDPWD=")), custom_putchar('\n');
								            chdir_ret = /* TODO: what should this be? */
										                chdir((dir = _getenv(info, "OLDPWD=")) ? dir : "/");
									        }
				    else
					            chdir_ret = chdir(info->argv[1]);
				        if (chdir_ret == -1)
						    {
							            print_error(info, "can't cd to ");
								            error_puts(info->argv[1]), error_putchar('\n');
									        }
					    else
						        {
								        _setenv(info, "OLDPWD", _getenv(info, "PWD="));
									        _setenv(info, "PWD", getcwd(buffer, 1024));
										    }
					        return (0);
}

/**
 *  * custom_help - changes the current directory of the process
 *   * @info: Structure containing potential arguments. Used to maintain
 *    * constant function prototype.
 *     * Return: Always 0
 *      */
int custom_help(CommandInfo_t *info)
{
	    char **arg_array;

	        arg_array = info->argv;
		    custom_puts("help call works. Function not yet implemented \n");
		        if (0)
				        custom_puts(*arg_array); /* temp att_unused workaround */
			    return (0);
}

