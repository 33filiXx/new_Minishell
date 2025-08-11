/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-mjiy <wel-mjiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 22:00:19 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/08/11 02:15:52 by wel-mjiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nrc/minishell.h"

void print_node(t_command *head)
{
    while (head)
    {
        printf("----- Command Node -----\n");

        // Print argv
        int i = 0;
        while (head->argv && head->argv[i])
        {
            printf("argv[%d] = [%s]\n", i, head->argv[i]);
            i++;
        }

        // Print redirections (use a temp pointer so we don't lose original list)
        t_redirection *redir = head->redirs;
        while (redir)
        {
            int type = redir->type;
            char *smiyadyaltype = NULL;
            switch (type)
            {   
            case 1:
                smiyadyaltype = "REDIR_IN";
                break;
            case 2:
                smiyadyaltype = "REDIR_OUT";
                break;
            case 3:
                smiyadyaltype = "REDIR_APPEND";
                break;
            case 4:
                smiyadyaltype = "HEREDOC";
                break;
            default:
                break;
            }
            printf("redir: { type: %s, filename: %s }\n", smiyadyaltype, redir->filename);
            redir = redir->next;
        }
        printf("path: %s\n----------------",head->path);
        
        // Print pipe info
        printf("pipe_in: %d, pipe_out: %d\n", head->pipe_in, head->pipe_out);

        printf("----------------------------------------------------------------------------------------\n\n\n");

        head = head->next;
    }
}


void free_env(t_env *env)
{
    t_env *temp;

    while (env)
    {
        temp = env;
        env = env->next;
        free(temp->key);   // Free the key string
        free(temp->value); // Free the value string
        free(temp);        // Free the node itself
    }
}


void print_nodee (t_lexer *head)
{
    if(!head)
        return;
   
    while (head)
    {
    int i = 0;
    int max_len = head->lenght_q > 0 ? head->lenght_q : 1;
    while (i < max_len)
    {
        printf("q : %d\n", head->q[i]); 
        i++;
    }
    i = 0;
    while (i < max_len)
    {
        printf("lenght_single : %d\n", head->lenght_single[i]); 
        i++;
    }
        printf("%s: ->>>>>>>>>>>>> %u ->>>>>>>>>>>>>>>>>>>>>>> %u -------->%d--->%d :\n" ,  head->content ,head->token , head->quotes ,  head->lenght_double[0] , head->lenght_normal);
        head = head->next;
    }
}

int *exit_stat(void)
{
    static int stat;

    return(&stat);
}
int main(int argc, char *argv[], char **envp)
{
    (void)argv;
    (void)argc;
    
    char *input;
    t_lexer *lexer_list = NULL;
    t_command *command = NULL;
        
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
    t_env *env = init_env(envp);
    if (!env)
        init_minimal_env(&env);
    else
        update_shlvl(&env);
    // heredoc("walid");
    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
            ctrl_d_handle(env);
        if (*input)
            add_history(input);
        
        if (lexer(input, &lexer_list))
        {
            ft_free_lexer_list(lexer_list);
            ft_free_command_list(command);
        }
        else
        {
        char **env_char = list_to_char_array(env);
            print_nodee(lexer_list);
            exit(1);
       expand(env , lexer_list);
        int parser_status = parser(lexer_list, &command, env_char );
        // print_node(command);
        free_split(env_char);
        if (parser_status == 0 && command)
        {
            if (command->pipe_in || command->pipe_out)
                execute_pipeline(command, env);
            else
                init_exc(command, &env);
            ft_free_command_list(command);
            command = NULL;
        }
        else
        {
            ft_free_command_list(command);
            command = NULL;
        }
        ft_free_lexer_list(lexer_list);
        lexer_list = NULL;
        free(input);
        }
    }
    // free(lexer_list->lenght_double);
    write_history(".minishell_history");
    free_env(env);
    return (*exit_stat());
}
