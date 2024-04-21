/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:16:32 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/20 11:16:33 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	free_str(char **p);
void    free_cmds(void *ptr);


bool    valid_env_char(char c);


void ms_exit(t_params *params, int code)
{

	if (params->paths)
	{
        for (int i = 0; params->paths[i]; i++)
        {
            // printf("%s\n", params->paths[i]);
            free(params->paths[i]);
        }

        free(params->paths);
	}

	ft_lstclear(&(params->cmd_list), free_cmds);

	int i = -1;
	while (__environ[++i])
		free_str(__environ + i);
	free(__environ);

	free_str(&(params->line));

	rl_clear_history();
	// close(params->tty_io[0]);
	// close(params->tty_io[1]);
	exit(code);
}

void unset_env(char *var)
{
	char *key = ft_strdup(var);
	char *loc = ft_strchr(key, '=');
	if (loc) *loc = '\0';  // split the key and value

	// check if key exists in environ
	// if yes, free and replace

	int i = 0;
	while (__environ[i])
	{
		if (!ft_strncmp(__environ[i], key, ft_strlen(key)))
		{
			__environ[i][ft_strlen(key)] = '\0';
			break ;
		}
		i++;
	}
	free(key);
}

void set_env(char *var)
{
    char *key = ft_strdup(var);
    char *loc = ft_strchr(key, '=');
    if (loc) *loc = '\0';  // split the key and value

	// check if key exists in environ
	// if yes, free and replace

	int i = 0;
	while (__environ[i])
	{
		if (!ft_strncmp(__environ[i], key, ft_strlen(key)))
		{
			free(__environ[i]);
			__environ[i] = ft_strdup(var);
			break ;
		}
		i++;
	}
	if (!__environ[i])
	{
		__environ = ft_realloc(__environ, i * sizeof(char *), (i + 2) * sizeof(char *));
		__environ[i] = ft_strdup(var);
		__environ[i + 1] = NULL;
	}
	free(key);

}

int ms_export(char *arg)
{
	char *key = NULL;
	char *value = NULL;
	char *equals_sign = NULL;
	bool valid = true;


	equals_sign = ft_strchr(arg, '=');
    if (equals_sign && equals_sign != arg)
	    key = ft_substr(arg, 0, equals_sign - arg);
    else
        key = ft_strdup(arg);

	int i = -1;
    while (key[++i])
		if (!valid_env_char(key[i]))
			valid = false;

	if (!valid)
	{
        ft_putstr_fd("export: '", 2);
		ft_putstr_fd(key, 2);
		ft_putstr_fd("' not a valid identifier\n", 2);
	}
	else if (equals_sign)
	{
        value = ft_strdup(equals_sign + 1);
		if (valid && value != NULL)
				set_env(arg);
	}

	free_str(&key);
	free_str(&value);
    return (!valid);
}

void print_env(void)
{
    int i = 0;
    while (__environ[i])
    {
        if (ft_strncmp(__environ[i], "?", 1) && ft_strchr(__environ[i], '='))
            printf("%s\n", __environ[i]);
        i++;
    }
}

int run_builtin(t_params *params, t_list *cmd_lst)
{
	t_cmd   *cmd = cmd_lst->content;
	char    **argv = cmd->words;
    int     code = 0;

	if (!ft_strncmp(argv[0], "export", 7))
	{
		int i = 0;
		while (argv[++i])
			code |=  ms_export(argv[i]);
	}
	else if (!ft_strncmp(argv[0], "env", 4))
	{
		print_env();
	}
	else if (!ft_strncmp(argv[0], "exit", 5))
	{
        if (params->num_cmds == 1 && !cmd_lst->next)
            printf("exit\n");
		ms_exit(params, 0);
	}
	else if (!ft_strncmp(argv[0], "unset", 6))
	{
		// run unset
		int i = 1;
		while (argv[i])
		{
			unset_env(argv[i]);
			i++;
		}
	}
    printf("\t\tbuiltin exitin with code %i\n", code);
    return code;
}