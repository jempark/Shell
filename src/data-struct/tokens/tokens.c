/* This file was edited and taken from Jemin Park's submission from HW05 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "tokens.h"
#include "svec.h"

bool
checkOperand(char c) 
{
    char operands[] = {'<', '>', '&', '|', ';', '(', ')'};
    for (int i = 0; i < 7; i++) {
        if (operands[i] == c) {
            return true;
        }
    }

    return false;
}

char*
parseCommand(char* string) 
{
    if (checkOperand(string[0])) {
        if ((string[0] == '|' && string[1] == '|') ||
            (string[0] == '&' && string[1] == '&')) {
                return strndup(string, sizeof(char) * 2);
        }
        else {
            return strndup(string, sizeof(char) * 1);
        }
    } 
    else {
        int i = 0;
       
        while (!checkOperand(string[i]) && !isspace(string[i])) {
            i++;
        }

        return strndup(string, sizeof(char) * i);
    }
}

svec* 
tokenize(char* line) 
{
    svec* xs = make_svec(); 
    int i = 0;

    char* string, *token;

    while (i < strlen(line)) {
        if (isspace(line[i])) {
            i++;
        } 
        else {
            string = &line[i];
            token = parseCommand(string);
        
            svec_push_back(xs, token);
            i += strlen(token);

            free(token);
        }
    }   

    return xs;
}

