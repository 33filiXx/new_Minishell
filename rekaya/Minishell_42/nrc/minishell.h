/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 21:59:54 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/11 16:20:48 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../../ft_libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>


# define REDIR_IN 1     // <
# define REDIR_OUT 2    // >
# define REDIR_APPEND 3 // >>
# define HEREDOC 4      // <<

typedef struct s_stor_quotes
{
	int						i;
	int						s;
	int						stalker;
	int						j;
	int						checker_quotes;
	char					*tmp;
}							s_stor_quotes;

typedef enum t_tokens
{
	PIPE,   // |
	TRUNC,  // >
	HERDOC, // <<
	APPEND, // >>
	INPUT,  //  <
	WORD,   // commend
}							e_tokens;
typedef enum e_dollar
{
	on,
	off,
	edge,
}							t_dollar;

typedef enum s_quotes
{
	quotes,
	no_quotes,
}							t_quotes;

typedef struct s_store_helper
{
	int						state_double;
	int						state_single;
	int						last_quotes;
	int						check;
}							t_store_helper;
typedef struct s_rebuild_var
{
	int		lenght_two;
	char	*result;
	int j;
	int p;
	int check ;
	int lenght_one;
}							t_rebuild_var;

typedef struct s_expand_var
{
	int			check;
	int			edge;
	char		*newstr;
	char		*result;
	char		*finale_r;
	char		*str;	
	int			i;
	t_dollar	*dollar;
	int			lenght;
}							t_expand_var;

typedef struct s_lexer
{
	e_tokens				token;
	t_quotes				quotes;
	char					*content;
	int						*q;
	int						lenght_q;
	int						*lenght_double;
	int						*lenght_single;
	int						lenght_normal;
	struct s_lexer			*next;
}							t_lexer;

typedef struct s_redirection
{
	int type;       // 1: <, 2: >, 3: >>, 4: << mt3l9a bdik define li dart lik
	char *filename; // e.g., "output.txt"'
	struct s_redirection	*next;
}							t_redirection;

typedef struct s_command
{
	char **argv;           // e.g., {"grep", "hello", NULL}
	t_redirection *redirs; // linked list of redirections
	char					*path;
	int pipe_in;                      
		// 1 if input is from previous pipe tanchra7om lik hado ra shalin
	int pipe_out;           // 1 if output goes to next pipe
	struct s_command *next; // next command in pipe chain
}							t_command;



typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
}	t_env;

typedef struct s_pipeline_data
{
	int		count;
	int		**pipes;
	pid_t	*pids;
	char	**envp;
}	t_pipeline_data;


typedef struct s_redirect {
    char *type;       // e.g., "<<"
    char *delimiter;  // e.g., "EOF"
    char *filename;   // e.g., ".heredoc_tmp"
} t_redirect;


t_lexer						*creat_node(char *content, e_tokens token,
								t_quotes quotes);
void						insert_at_end(t_lexer **head, char *content,
								e_tokens token, t_quotes quotes);
int						lexer(char *argv, t_lexer **lexer_list);
void						ft_free_nodes(t_lexer *lexer);
int							ft_strcmp(const char *s1, const char *s2);
int							parser(t_lexer *lexer, t_command **command_list,
								char **envp );
void						expand(t_env *env, t_lexer *lexer);
void						ft_free_lexer_list(t_lexer *head);
void						ft_free_command_list(t_command *head);
char 						*heredoc_handle(t_redirection **redir, char **dof , char **envp , t_lexer **lexer);
void 						heredoc_parent(t_redirection **redir, char **dof, char **envp, t_lexer **lexer);
char						*cleanup_argv(char **str, t_lexer **lexer, t_expand_var *expand);
void						free_one_lexer(t_lexer *lexer);


// exec builtin
int		echo_builtin(char **args);
int		pwd_builtin(t_env *env);
int		cd_builtin(t_env *env, char **args);
int		export_builtin(t_env **env, char **args);
int		is_valid_identifier(char *s);
int		exit_builtin(char **argv, int abi);
void	print_env_export(t_env *env);
void	unset_env(t_env **env, const char *key);
	
//env_handel
t_env	*env_node_new(const char *key, const char *value);
void    env_add_back(t_env **lst, t_env *new);
t_env	*init_env(char **envp);
char	*get_env_value(t_env *env, const char *key);
void	set_env_value(t_env **env, const char *key, const char *value);
void	unset_env(t_env **env, const char *key);
void    print_env(t_env *env);
void	init_minimal_env(t_env **env);
void	update_shlvl(t_env **env_list);

// memory_management
void	free_split(char **s);
void	free_redirections(t_redirection *redirs);
void	free_commend(t_command *cmd);
int		*exit_stat(void);
void	ft_free_env(t_env *head);

//exc_start
int		init_exc(t_command *cmd, t_env **env);
int		excute_commend(t_command *cmd, char **env);
char	*extract_path(char *cmd, char **envp , t_command **command_list);
void	get_command_path(char **cmd, char **envp, t_command **command_list);
char	**list_to_char_array(t_env *list);
int		redirections(t_command *cmd);
void	close_all_pipes(int **pipes, int count);
int		wait_for_child(pid_t child_pid);
int		ft_lssize(t_env *lst);

// exc helpers
void	print_command_error(char *cmd_name, char *error_msg);
void	check_path_exists(t_command *cmd);
void	check_file_access(t_command *cmd);
void	handle_command_errors(t_command *cmd);
void	execute_child_process(t_command *cmd, char **env);
int		handle_parent_process(pid_t pid1);
int		execute_external_command(t_command *cmd, t_env **env);
int		execute_builtin_commands(t_command *cmd, t_env **env);
int		handle_redirection_only(t_command *cmd);
int		setup_builtin_redirections(t_command *cmd, int *save_stdin,
	int *save_stdout);
void	restore_redirections(int save_stdin, int save_stdout);
int		execute_command_logic(t_command *cmd, t_env **env, int builtin);
int		handle_initial_checks(t_command *cmd);
int		handle_builtin_execution(t_command *cmd, t_env **env,
	int *save_stdin, int *save_stdout);
int		handle_unset_command(t_command *cmd, t_env **env);
int		handle_exit_command(t_command *cmd, int abi);

// pipeline helpers
int		execute_pipeline(t_command *cmd, t_env *env);
int		setup_pipeline_data(t_command *cmd, t_env *env, t_pipeline_data *data);
int		create_pipes(t_pipeline_data *data);
void	child_process(t_command *cmd, t_pipeline_data *data, int i, t_env *env);
int		fork_and_execute(t_command *cmd, t_env *env, t_pipeline_data *data);
int		wait_for_all_children(t_pipeline_data *data);
int		count_commands(t_command *cmd);
void	cleanup_pipeline(int **pipes, int pipes_allocated, pid_t *pids,
			char **envp);

//signals
void	ctrl_d_handle(t_env *env);
void	sigint_handler(int sig);
void	heredoc_sigint_handler(int sig);
void	sigint_han_p(int sig);
void	child_sigint_handler(int sig);
void	sig_herdoc(int sig);

#endif