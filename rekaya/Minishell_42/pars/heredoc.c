/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 07:42:53 by wel-mjiy          #+#    #+#             */
/*   Updated: 2025/08/09 21:49:44 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"


static void	reset_rebuild_var(t_rebuild_var *rebuild, char *lexer, char *env)
{
	rebuild->check = 0;
	rebuild->j = 0;
	rebuild->p = 0;
	rebuild->lenght_one = ft_strlen(lexer);
	rebuild->lenght_two = ft_strlen(env);
}

char	*namegen(void)
{
	int		fd;
	char	*file;
	char	*buf;
	int		valid_data;
	int		j;

	valid_data = 0;
	fd = open("/dev/random", O_RDONLY);
	file = malloc(11);
	buf = malloc(1);
	if (!file || !buf)
	{
		return (free(file), free(buf), NULL);
	}
	j = 0;
	while (valid_data <= 0)
	{
		valid_data = read(fd, buf, 1);
		if (ft_isascii(*buf) || *buf != '.')
			file[j++] = *buf;
	}
	if (valid_data < 0)
		return (NULL);
	file[j] = '\0';
	close(fd);
	return (file);
}

char	*randgen(char *s)
{
	char	*pathname;

	pathname = "/tmp/";
	// printf("-->  %s\n" , namegen());
	s = ft_strjoin(pathname, namegen());
	return (s);
}

int	file_to_write_on(char **filename)
{
	char	*path;
	int		fd;

	path = randgen(*filename);
	fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0777);
	*filename = path;
	return (fd);
}
int	heredoc_strlen(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && s[i] != '=')
	{
		i++;
	}
	return (i);
}

char	*ft_newdup(const char *s)
{
	int		i;
	char	*p;

	if (!s)
		return (NULL);
	p = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!p)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != '=')
	{
		p[i] = s[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

char	*get_value(char *get)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	j = 0;
	line = malloc(ft_strlen(get) + 1);
	while (get[i] && get[i] != '=')
		i++;
	i++;	
	while (get[i])
		line[j++] = get[i++];
	line[j] = '\0';
	return (line);
}

int	lenght_of_lien(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] != ' ' && ft_isalpha(s[i]) == 1)
		i++;
	return (i);
}

static char	*my_strdup(const char *s)
{
	int		i;
	char	*p;

	if (!s)
		return (NULL);
	p = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!p)
		return (NULL);
	i = 0;
	// printf("%d----%s\n", *((*lexer)->lenght_double), s);
	// exit(1);
	while (i < lenght_of_lien(s))
	{
		if (ft_isalpha(s[i]) != 1)
			break ;
		p[i] = s[i];
		i++;
	}
	p[i] = '\0';
	// printf("%s----\n", p);
	return (p);
}

static void	clean_up_helep(char *str, char **newstr)
{
	int	i;

	i = 0;
	if (str[i] == '$' && str[i + 1] != '\'' && str[i + 1] != '"' && str[i
		+ 1] != '\0')
	{
		while (str[i])
		{
			if (str[i] == '$')
			{
				i++;
				if (str[i] == '?')
				{
					*newstr = ft_strdup("?");
					break ;
				}
				*newstr = my_strdup(&str[i]);
				break ;
			}
			i++;
		}
	}
}

static char	*clean_argv(char *str)
{
	char	*newstr;

	newstr = NULL;
	clean_up_helep(str, &newstr);
	if (newstr)
		return (newstr);
	newstr = ft_strdup("\0");
	return (newstr);
}

static void	update_lexer(char **str)
{
	if (**str == '$')
		(*str)++;
	while (**str)
	{
		if (ft_isalpha(**str) != 1)
			break ;
		(*str)++;
	}
}

static int	alloc_rebuild(t_rebuild_var **rebuild, char *lexer, char *env,
		char **result)
{
	*rebuild = malloc(sizeof(t_rebuild_var));
	if (!(*rebuild))
		return (1);
	reset_rebuild_var(*rebuild, lexer, env);
	*result = malloc(((*rebuild)->lenght_one + (*rebuild)->lenght_two) + 1);
	if (!(*result))
	{
		free(rebuild);
		return (1);
	}
	return (0);
}
static char	*rebuild_and_stor(char **lexer, char *env)
{
	char			*result;
	t_rebuild_var	*rebuild;

	if (alloc_rebuild(&rebuild, *lexer, env, &result))
		return (NULL);
	while (**lexer)
	{
		if (**lexer == '$' && rebuild->check == 0)
		{
			while (env[rebuild->p])
				result[rebuild->j++] = env[rebuild->p++];
			update_lexer(lexer);
			rebuild->check = 1;
			continue ;
		}
		if (**lexer == '$' || **lexer == '\'' || **lexer == '"')
			break ;
		result[rebuild->j++] = *(*lexer)++;
	}
	result[rebuild->j] = '\0';
	return (result);
}

static void	update_to_dollar(char **str)
{
	if (**str == '$')
		(*str)++;
	while (**str)
	{
		// if (**str == '$' && **(str + 1) == '?')
		// {
		// 	// *str = *(str + 2);
		// 	(*str)++;
		// 	// (*str)++;
		// 	break ;
		// }
		if (ft_isalpha(**str) == 0)
			break ;
		(*str)++;
	}
}
static void	update_exit(char **str)
{
	if (**str == '$')
		(*str)++;
	if (**str == '?')
		(*str)++;
}
void	expand_line(char **line, char **envp)
{
	int		i;
	char	*new_line;
	char	*finale_line;
	char	*clean_line;
	char	*result;
	char	one[2];
	char *finale_result;
	// char *valuee;
	
	new_line = NULL;
	finale_line = *line;
	finale_result = NULL;
	clean_line = NULL;
	// valuee = NULL;
	char **tmp = line;
	while (*finale_line)
	{
		clean_line = clean_argv(finale_line);
		result = NULL;
		tmp = envp;
		i = 0;
		while (tmp[i])
		{
			new_line = ft_newdup(tmp[i]);
			// printf("key :%s\n" , new_line);
			// printf("clean :%s\n" , clean_line);
			// printf("value :%s\n" , get_value(envp[i]));
			// printf("line :%s\n" , finale_line);
			// exit(1);
			if (ft_strcmp(new_line, clean_line) == 0)
			{				
				result = rebuild_and_stor(&finale_line, get_value(tmp[i]));
				finale_result = ft_strjoin(finale_result , result);
				// printf("R :%s\n" , finale_line);
				// exit(1);
				break;
			}
			i++;
		}
		if (ft_strcmp(clean_line , "\0") == 0)
		{
			while (*finale_line)
			{
				if (*finale_line == '$')
				{
					break ;
				}
				one[0] = *finale_line;
				one[1] = '\0';
				result = ft_strjoin(result, one);
				finale_line++;
			}
			// update_to_dollar(line);
		}
		else
		{
			// printf("%s\n" , clean_line);
			// exit(1);
			if(ft_strcmp(clean_line , "?"))
				update_to_dollar(&finale_line);
			else
			{
				// printf
				result = ft_itoa((*exit_stat()));
				update_exit(&finale_line);				
			}
		}
		finale_result = ft_strjoin(finale_result , result);
	}
	// printf("R :%s\n" , finale_result);
	// exit(1);
	*line = finale_result;
}

char *heredoc_handle(t_redirection **redir, char **dof, char **envp,
	t_lexer **lexer)
{
	(void)redir;
	int		i;
	char	*filename;
	char	*line;
	int		fd;
	char **tmp = dof;
	
	i = 0;
	fd = file_to_write_on(&filename);

	while (1)
	{
		line = readline("Heredoc >");
		if(line && ft_strcmp(line, dof[i]) == 0)
		{
			i++;
		}
		if (!line)
		{
			dof = tmp;
			// (*lexer) = (*lexer)->next;
			close(fd);
			return (filename);
		}
		else if ((*lexer)->quotes == 1 && ft_strcmp(line , dof[i]))
			expand_line(&line, envp);
		if (!ft_strcmp(dof[i] ,"\0"))
		{
			dof = tmp;
			// (*lexer) = (*lexer)->next;
			close(fd);
			return (filename);	
		}
		if (!ft_strcmp(dof[i + 1] , "\0"))
		{
			write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);	
		}
	}
}

void heredoc_parent(t_redirection **redir, char **dof, char **envp, t_lexer **lexer)
{
    pid_t pid;
    int status;
    int pipefd[2];
    char *filename;
    ssize_t bytes_read;

    if (pipe(pipefd) == -1)
        return;

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    pid = fork();
    if (pid == 0)
    {
        close(pipefd[0]);
        signal(SIGINT, sig_herdoc);
        signal(SIGQUIT, SIG_IGN);
        filename = heredoc_handle(redir, dof, envp, lexer);
        if (filename)
			write(pipefd[1], filename, ft_strlen(filename));
        close(pipefd[1]);
        exit(0);
    }
    else
    {
        close(pipefd[1]);
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
        {
            *exit_stat() = 130;
            (*redir)->filename = NULL;
            close(pipefd[0]);
            return;
        }
        filename = malloc(1024);
        bytes_read = read(pipefd[0], filename, 1023);
        if (bytes_read > 0)
        {
            filename[bytes_read] = '\0';
            (*redir)->filename = filename;
        }
        else
        {
            free(filename);
            (*redir)->filename = NULL;
        }
        close(pipefd[0]);
    }
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}
