#include <string.h>
#include <stdlib.h>
#include <stdio.h>
    
#include "ast.h"
#include "svec.h"

ast*
make_ast_cmd(svec* cmd) 
{
    ast* tree = malloc(sizeof(ast));
    tree->op = "$";
    tree->cmd = cmd;
    tree->left = NULL;
    tree->right = NULL;
    
    return tree;
}

ast*
make_ast_op(char* op, ast* left, ast* right) 
{
    ast* tree = malloc(sizeof(ast));
    tree->op = op;
    tree->cmd = 0;
    tree->left = left;
    tree->right = right;
    
    return tree; 
}

int 
checkOperands(svec* tokens)
{
    char* operands[9] = {"<", ">", "&", "|", ";", "&&", "||", ")", "("};
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < tokens->size; j++) {
            if (strcmp (svec_get(tokens, j), operands[i]) == 0) {
                return j; 
            }
        }
    }
    
    return -1; 
}

ast* 
parse(svec* tokens)
{
    int i = checkOperands(tokens); 

    if (i != -1) {
        svec* left = make_svec();
        svec* right = make_svec();

        for (int x = 0; x < i; x++) {
            svec_push_back(left, strdup(svec_get(tokens, x)));
        }
        for (int x = i + 1; x < tokens->size; x++) {
            svec_push_back(right, strdup(svec_get(tokens, x)));
        }   

        return make_ast_op(strdup(svec_get(tokens, i)), parse(left), parse(right));
    } 
    else {
        return make_ast_cmd(tokens);
    }
}

void 
free_ast(ast* tree)
{
    if (tree) {
        if (tree->cmd) {
            free_svec(tree->cmd);
        }
        if (tree->left) {
            free_ast(tree->left);
        }
        if (tree->right) {
            free_ast(tree->right);
        }
        free(tree);
    }
}
