/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:29:44 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/20 11:29:45 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int     is_redirect(char *line);
bool    is_meta(char *line);
bool    is_space(char c);
bool 	valid_env_start(char *line);	
bool 	valid_env_char(char c);
t_redir_type get_redir_type(char *line);

void	free_str(char **p);


bool pipe_syntax_error(char *line, char *start)
{
	char *ptr;

	ptr = line;
	if (ptr == start || !ptr[1])
		return (true);
	ptr--;
	while (is_space(*ptr))
	{
		if (ptr == start)
			return (true);
		ptr--;
	}
	if (*ptr == '|')
		return (true);
	ptr = line + 1;
	while (is_space(*ptr))
	{
		ptr++;
		if (!(*ptr))
			return (true);
	}
	if (*ptr == '|')
		return (true);
	return (false);
}

bool redir_syntax_error(char **line_ptr)
{
	char *line;

	if (!ft_strncmp((*line_ptr), "<<", 2) || !ft_strncmp((*line_ptr), ">>", 2))
		(*line_ptr)++;
	line = ++(*line_ptr);
	while (is_space(*line))
	{
		line++;
		if (!(*line))
			return (true);
	}
	if (!line[0] || is_meta(line))
		return (true);
	return (false);
}

bool invalid_syntax(char **line_ptr, int *cmd_count, char *start)
{
	char *line;

	line = *line_ptr;
	if (*line == '\'' || *line == '"')
	{
		if (!(line[1]))
			return (true);
		*line_ptr = ft_strchr(line + 1, *line);
		if (!(*line_ptr))
			return (true);
	}
	else if (*line == '|')
	{
		if (pipe_syntax_error(line, start))
			return (true);
		(*cmd_count)++;
	}
	else if (is_redirect(line))
		if (redir_syntax_error(line_ptr))
			return(true);
	return (false);
}

int	count_cmds(char *line)
{
	int		cmd_count;
	char	*start;
	char	*ptr;

	while (is_space(*line))
		line++;
	if (!line[0])
		return (0);
	cmd_count = 1;
	start = line;
	while (*line)
	{
		if (invalid_syntax(&line, &cmd_count, start))
		{
			ft_putstr_fd("Syntax error\n", 2);
			return (-1);
		}
		else if (*line)
			line++;
	}
	return cmd_count;
}

char *get_env_key(char *line)
{
	char *result;

	int i = 1;
	if(line[i] == '?')
		return ft_substr(line, 1, 1);
	if (line[i] == '_' || ft_isalpha(line[i]))
	{
		i++;
		while (valid_env_char(line[i]))
			i++;
		result = ft_substr(line, 1, i - 1);
		return result;
	}

	return (NULL);
}



int		len_to_alloc(char **line_ptr, char qstart)
{
	if (!qstart && (!**line_ptr || is_redirect(*line_ptr) || is_space(**line_ptr)))
		return (0);

	if (qstart && **line_ptr == qstart)
	{
		(*line_ptr)++;
		qstart = 0;
	}

	if (!qstart && (!**line_ptr || is_redirect(*line_ptr) || is_space(**line_ptr)))
		return (1);
	else if (!qstart && **line_ptr == '\'')
	{
		(*line_ptr)++;
		return (len_to_alloc(line_ptr, '\''));
	}
	else if (!qstart && **line_ptr == '"')
	{
		(*line_ptr)++;
		return (len_to_alloc(line_ptr, '"'));

	}
	else if (qstart != '\'' && valid_env_start(*line_ptr))
	{
		char *var = get_env_key(*line_ptr);
		int	key_len = 0;
		int len = 0;

		// printf("\t\t VAR: %s\n", var);
		char *value = getenv(var);
		if (value)
		{
			// printf("\t\t VALUE: %s\n", value);
			len = ft_strlen(value);
		}
		key_len = ft_strlen(var) + 1;
		*line_ptr += key_len;
		free_str(&var);
		// printf("\t\t env var len to alloc: %i\n", len);
		return (len + len_to_alloc(line_ptr, qstart));
	}
	else
	{	
		(*line_ptr)++;
		return (1 + len_to_alloc(line_ptr, qstart));
	}
}

void	word_copy(char **line_ptr, char qstart, char *word)
{
	if (qstart && **line_ptr == qstart)
	{
		(*line_ptr)++;
		qstart = 0;
	}

	if (!qstart && (!**line_ptr || is_redirect(*line_ptr) || is_space(**line_ptr)))
	{
		// size = 0;
		return ;
	}

	else if (!qstart && **line_ptr == '\'')
	{
		(*line_ptr)++;
		return (word_copy(line_ptr, '\'', word));
	}
	else if (!qstart && **line_ptr == '"')
	{
		(*line_ptr)++;
		return (word_copy(line_ptr, '"', word));
	}
	else if (qstart != '\'' && valid_env_start(*line_ptr))
	{
		char *var = get_env_key(*line_ptr);
		int	key_len = 0;
		int len = 0;

		// printf("\t\t VAR: %s\n", var);
		char *value = getenv(var);
		if (value)
		{
			// printf("\t\t VALUE: %s\n", value);
			len = ft_strlen(value);
			ft_strlcpy(word, value, len + 1);
			word += len;
		}
		key_len = ft_strlen(var) + 1;
		*line_ptr += key_len;
		free_str(&var);
		// printf("\t\t env var len to alloc: %i\n", len);
		return (word_copy(line_ptr, qstart, word));
	}
	else
	{
		*word = **line_ptr;
		(*line_ptr)++;
		word++;
		return (word_copy(line_ptr, qstart, word));
	}
}


void parse_cmd(t_list *cmd_lst)
{
	t_cmd *cmd = (t_cmd *) cmd_lst->content;
	char *line = cmd->line;
	// printf("line : %s\n", cmd->line);
	// int i = 0;

	char **words = ft_calloc(1, sizeof(char **));
	// char **redirects = ft_calloc(1, sizeof(char **));
	t_redir *redirs = cmd->redirs;
	while (*line)
	{
		if (*line && is_space(*line))
		{
			line++;
			continue ;
		}
		if (is_redirect(line))
		{
			t_redir_type type = get_redir_type(line);


			if (type == heredoc)
				cmd->num_heredocs++;
			// skip and process redirect
			// since we have verified that it isn't final
			line += is_redirect(line);
			while (is_space(*line))
				line++;
			char *copy = line;
			int len = len_to_alloc(&line, 0);
			// printf("allocating pointer of len %i\n", len);
			// printf("line is crrently %s\n", line);
			redirs = ft_realloc(redirs, cmd->num_redirects * sizeof(t_redir), (cmd->num_redirects + 1) * sizeof(t_redir));
			char *redir_file = ft_calloc(len + 1, sizeof(char));
			word_copy(&copy, 0, redir_file);

			redirs[cmd->num_redirects].file = redir_file;
			redirs[cmd->num_redirects].type = type;

			cmd->num_redirects++;
			// printf("redirect len to alloc: %i\n", len);
		}
		else
		{
			char *copy = line;
			int len = len_to_alloc(&line, 0);
			if (len)
			{
				// char *word = ft_calloc(len, sizeof(char));
				// printf("allocating pointer of len %i\n", len);
				// printf("line is crrently %s\n", line);
				char *word = ft_calloc(len + 1, sizeof(char));
				words[cmd->num_words] = word;
				word_copy(&copy, 0, words[cmd->num_words]);
				cmd->num_words++;
				words = ft_realloc(words, cmd->num_words * sizeof(char*), (cmd->num_words + 1) * sizeof(char*));
			}
		}
		// printf("i: %i. line: %s\n", i,  line + i);
	}
	cmd->words = words;
	cmd->redirs = redirs;
	// printf("num words: %i. num redirects: %i\n", cmd->num_words, cmd->num_redirects);
}


void create_cmds(t_params *params)
{
	int total_len;
	char *line;
	char *start;

	if (!params)
		return ;
	line = params->line;

	if (!line)
		return ;

	params->total_heredocs = 0;
	total_len = 0;
	start = line;
	while (*line)
	{
		if (*line == '\'')
			line = ft_strchr(line + 1, '\'');
		else if (*line == '"')
			line = ft_strchr(line + 1, '"');
		else if (*line == '|')
		{

			t_cmd *cmd = ft_calloc(1, sizeof(t_cmd));
			t_list *node = ft_calloc(1, sizeof(t_list));

			int len = line - (start + total_len);

			// printf("len : %i\n", len);
			cmd->line = ft_substr(params->line, total_len, len);
			total_len += len + 1;

			node->content = cmd;
			parse_cmd(node);
			ft_lstadd_back(&params->cmd_list, node);
		}

		line++;
		if (!line[0])
		{
			t_cmd *cmd = ft_calloc(1, sizeof(t_cmd));
			t_list *node = ft_calloc(1, sizeof(t_list));

			int len = line - (start + total_len);

			// printf("total cmd len : %i\n", len);
			cmd->line = ft_substr(params->line, total_len, len);
			total_len += len + 1;

			node->content = cmd;
			parse_cmd(node);
			ft_lstadd_back(&params->cmd_list, node);
		}

	}
}

