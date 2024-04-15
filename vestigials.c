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