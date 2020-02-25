#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include "svec.h"

typedef struct ast { 
    char* op;
    svec* cmd; 
    struct ast* left; 
    struct ast* right;
} ast;

ast* make_ast_cmd(svec* cmd);
ast* make_ast_op(char* op, ast* left, ast* right);
ast* parse(svec* tokens);

void free_ast(ast* tree);

#endif 
