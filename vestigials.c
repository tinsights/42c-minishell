void copy_bytes(t_list *cmd_lst, t_list *env_lst)
{
	t_cmd *cmd = (t_cmd *) cmd_lst->content;
	char *line = cmd->line;
	// printf("line : %s\n", cmd->line);
	// int i = 0;

	int curr_word = 0;
	int curr_redirect = 0;
	while (*line)
	{
		if (*line && is_space(*line))
		{
			line++;
			continue ;
		}
		if (is_redirect(line))
		{

			line += is_redirect(line);
			while (is_space(*line))
				line++;

			t_redir redirect = cmd->redirs[curr_redirect];
			word_copy(&line, env_lst, 0, redirect.file);
			curr_redirect++;
		}
		else
		{
			char *word = cmd->words[curr_word];
			word_copy(&line, env_lst, 0, word);
			curr_word++;
		}
		// printf("i: %i. line: %s\n", i,  line + i);
	}
}

char *parse_env_var(t_list *env_lst, char *var)
{

	while (env_lst)
	{
		t_env *env_node = env_lst->content;

		if (!ft_strncmp(var, env_node->key, ft_strlen(var)))
		{
			// printf("%s\n", env_node->value);
			return (env_node->value);
		}

		env_lst = env_lst->next;
	}
	return (NULL);
}

void	create_words(t_params *params)
{
	/**
	 * TODO: Go through entire word,
	 * skip IFS/whitespace,
	 * if unquoted string
	 * malloc and add to **words arr
	 * if single quoted string
	 * remove quotes and add to **words
	 * if redirect operator,
	 * find filename and run redir prep
	*/
	t_list *cmd_lst = params->cmd_list;

	while (cmd_lst)
	{
		parse_cmd(cmd_lst); // cmd should point to env lst
		// copy_bytes(cmd_lst, env_lst);
		params->total_heredocs += ((t_cmd *) cmd_lst->content)->num_heredocs;
		cmd_lst = cmd_lst->next;
	}
}
