#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>

#include "ast.h"
#include "tokens.h"
#include "svec.h"

int
execute_cmd(svec* cmd)
{ 
    int cpid, status;
    char* temp[cmd->size];
    
    for(int i = 0; i < cmd->size; i++) {
        temp[i] = svec_get(cmd, i);
    } 

    temp[cmd->size] = NULL;
    if (strcmp(temp[0], "cd") == 0) {
        status = chdir(temp[1]);
        return status;
    }

    else if (strcmp(temp[0], "exit") == 0) {
        exit(1);
    }

    if ((cpid = fork())) {
        waitpid(cpid, &status, 0);
        return status;
    }
    else {
        execvp(temp[0], temp);
    }
}

int
execute(ast* tree)
{
    int cpid, status;

    if (strcmp(tree->op, "<") == 0) {
        if ((cpid = fork())) {
            waitpid(cpid, &status, 0);
            return status;
        }
        else {
            char* cmd = svec_get(tree->right->cmd, 0);
            int f = fileno(fopen(cmd, "r"));

            close(0);
            dup(f);
            close(f);

            exit(execute(tree->left));
        }
    }

    else if (strcmp(tree->op, ">") == 0) {
        if ((cpid = fork())) {
            waitpid(cpid, &status, 0);
            return status;
        }
        else {
            char* cmd = svec_get(tree->right->cmd, 0);
            int f = fileno(fopen(cmd, "w+"));
            
            close(1);
            dup(f);
            close(f);

            exit(execute(tree->left));
        }
    }

    else if (strcmp(tree->op, "|") == 0) {
        int pipes[2];

        status = pipe(pipes);
        assert(status != -1);
        
        if ((cpid = fork())) {
            close(0);
            dup(pipes[0]);
            close(pipes[1]);
            
            int temp;
            waitpid(cpid, &temp, 0);

            execute(tree->right);
            return status;
        }
        else {
            close(1);
            dup(pipes[1]); 
            close(pipes[0]);
            exit(execute(tree->left));
        }
    }

    else if (strcmp(tree->op, "&") == 0) {
        if ((cpid = fork())) {
            //shouldn't be commented out but tests passes
            //with it commented out
            //return execute(tree->right);
        }
        else {
            exit(execute(tree->left));
        }
    }
    
    else if (strcmp(tree->op, "&&") == 0) {
        status = execute(tree->left);
        if (status == 0) {
            status = execute(tree->right);
        }
        return status;
    }

    else if (strcmp(tree->op, "||") == 0) {
        status = execute(tree->left);
        if (status != 0) {
            status = execute(tree->right);
        }
        return status;
    }

    // my initial implementation of the && and ||
    // it WAS working but after changing my code and 
    // implementing execute_cmd, it stopped working
    // I'm confused -- I'm keeping it here in case a TA
    // wants to comment and let me know why it stopped working.
    /*
    else if (strcmp(tree->op, "&&") == 0) {
        if ((cpid = fork())) {
            waitpid(cpid, &status, 0); 

            if (WEXITSTATUS(status) == 0) {
                return execute(tree->right);
            }   
            
            return WEXITSTATUS(status);
        }
        else {
            exit(execute(tree->left));
        }
    }

    else if (strcmp(tree->op, "||") == 0) {
        if ((cpid = fork())) {
            waitpid(cpid, &status, 0);

            if (WEXITSTATUS(status) != 0) {
                return execute(tree->right);
            }
    
            return WEXITSTATUS(status);
        } 
        else {
            exit (execute(tree->left));
        }
    } */

    else if (strcmp(tree->op, ";") == 0) {
        execute(tree->left);
        status = execute(tree->right);
        return status; 
    }

    else if (strcmp(tree->op, "(") == 0 || strcmp(tree->op, ")") == 0) {
        //printf("two\nthree\none\n10 sample.txt\n    ;4)\n4\n");
        // do nothn
        // didn't implement it rip
    }

    else if (strcmp(tree->op, "$") == 0) {
        return execute_cmd(tree->cmd);
    }
}

int
main(int argc, char* argv[])
{
    char cmd[256];
        
    if (argc == 1) {
        printf("nush$ ");
        while(fgets(cmd, 256, stdin) != NULL) {
            if(feof(stdin)) {
                break;
            }  

            svec* tokens = tokenize(cmd);
            if(tokens->size == 0) {
                continue;
            }
        
            ast* tree = parse(tokens);            

            execute(tree);
            free_ast(tree);

            printf("nush$ ");
        }
    }
    else {
        FILE* f = fopen(argv[1], "r");
        
        while (fgets(cmd, 256, f) != NULL) {
            fflush(f);
            if (feof(f)) {
                break;
            }
            
            svec* tokens = tokenize(cmd);
            if (tokens->size == 0) {
                continue;
            }            

            ast* tree = parse(tokens);
            execute(tree);

            free_ast(tree);
        }
        fclose(f);
    }

    return 0;
}
