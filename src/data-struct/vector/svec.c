/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */
/* This file was edited and taken from Jemin Park's submission from HW04 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "svec.h"

svec*
make_svec()
{
    svec* sv = malloc(sizeof(svec));
    sv->data = malloc(2 * sizeof(char*));
    sv->size = 0;
    sv->cap = 2;

    return sv;
}

void
free_svec(svec* sv)
{
    for (int i = 0; i < sv->size; i++) {
	    free(sv->data[i]);
    }
    free(sv->data);
    free(sv);
}

char*
svec_get(svec* sv, int ii)
{
    assert(ii >= 0 && ii < sv->size);
    return sv->data[ii];
}

void
expand_vector(svec* sv) 
{
    sv->cap *= 2;
    sv->data = realloc(sv->data, sv->cap * sizeof(char*));
}

void
svec_put(svec* sv, int ii, char* item)
{
    assert(ii >= 0 && ii < sv->size);

    int newSize = sv->size + 1;

    if (sv->cap < newSize) {
	expand_vector(sv);
    }

    for(int x = sv->size; x > ii; x--) {
    	sv->data[x] = sv->data[x-1];
    }

    sv->data[ii] = strdup(item); 
}

void
svec_push_back(svec* sv, char* item)
{
    int ii = sv->size;
    int cap = sv->cap;

    if (ii == cap) {
	expand_vector(sv);
    }
    
    sv->size = ii + 1;
    svec_put(sv, ii, item);
}
