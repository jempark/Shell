// Some of the code that is written was based on in-class code from Jan 31st  
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <bsd/string.h>
#include <string.h>

#include "hashmap.h"

int
hash(char* key)
{
    int hashVal = 0;
    for (int i = 0; key[i]; i++) {
	hashVal = hashVal * 67 + key[i];
    }
    return hashVal;
}

hashmap_pair*
make_pair(const char* key, const int val)
{
    hashmap_pair* pp = malloc(sizeof(hashmap_pair));
    strlcpy(pp->key, key, 4);
    pp->val = val;
    pp->used = true;
    pp->tomb = false;

    return pp;
}

void 
free_pair(hashmap_pair* pp)
{
    if(pp) {
      free(pp);
    }
}

hashmap*
make_hashmap_presize(int nn)
{
    hashmap* hh = calloc(1, sizeof(hashmap));
    hh->size = 0;
    hh->cap = nn;
    hh->data = calloc(hh->cap, sizeof(hashmap_pair*));

    return hh;
}

hashmap*
make_hashmap()
{
    return make_hashmap_presize(4);
}

void
free_hashmap(hashmap* hh)
{
    for (int i = 0; i < hh->cap; i++) {
	if (hh->data[i]) {
	   free_pair(hh->data[i]);
	}
    }
    free(hh->data);
    free(hh);
}

int
hashmap_has(hashmap* hh, char* kk)
{
    return hashmap_get(hh, kk) != -1;
}

int
hashmap_get(hashmap* hh, char* kk)
{
    long i = hash(kk) & (hh->cap - 1);
    while (i < hh->cap) {
	hashmap_pair* x = hh->data[i];
	if (!x || !x->used) {
	    break;
	}
	else if (x->tomb) {
            i++; 
	    continue;
	}
	else if (strcmp(x->key, kk) == 0) {
	    return x->val;
	}
       	i++;
    }
    return -1;
}

void
hashmap_grow(hashmap* hh)
{
    int newSize = hh->cap * 2;
    hashmap_pair** temp = calloc(newSize, sizeof(hashmap_pair*));
    
    long hashValue = 0;

    for (int i = 0; i < hh->cap; i++) {
	 if (hh && hh->data[i] && hh->data[i]->tomb) {
	     free(hh->data[i]); 
	 }
	 else if (hh && hh->data[i] && hh->data[i]->used) {
	     hashValue = hash(hh->data[i]->key) & (newSize - 1);
             for (int y = hashValue; y < newSize; y++) {
		  if (temp && temp[y] && !temp[y]->used) {
                     temp[y] = hh->data[i];
		  }
	     }
	 }
    }
    
    free(hh->data);
    hh->data = temp;
    hh->cap *= 2;
}

void
hashmap_put(hashmap* hh, char* kk, int vv)
{
    if (hh->size >= hh->cap * .5) {
    	hashmap_grow(hh);
    }

    long i = hash(kk) & (hh->cap - 1);
    while (true) {
	if(!hh->data[i]) {
           hashmap_pair* y = make_pair(kk, vv);
	   hh->data[i] = y;
	   hh->data[i]->used = true;
	   hh->size += 1;
	   return;
	}

	hashmap_pair* x = hh->data[i % (hh->cap - 1)];
	if (!x || !x->used || !x->tomb) {
	    break;
	}
	else if (strcmp(x->key, kk) == 0) {
	    break; 
	}
	i++;
    }

    hashmap_pair* y = make_pair(kk, vv);
    hh->data[i] = y;
    hh->data[i]->used = true; 
    hh->size += 1;
}

void
hashmap_del(hashmap* hh, char* kk)
{
    long i = hash(kk) & (hh->cap - 1);
    while (i < hh->cap) {
        hashmap_pair* x = hh->data[i];
	if (strcmp(x->key, kk) == 0) {
	    x->tomb = true; 
	}
	i++;
    } 

}

hashmap_pair
hashmap_get_pair(hashmap* hh, int ii)
{
    return *hh->data[ii];
}

void 
print_pair(hashmap_pair* pp) {
    printf("key: %s\n", pp->key);
    printf("val: %d\n", pp->val);
    printf("used: %b\n", pp->used);
    printf("tomb: %b\n", pp->tomb);
}

void
hashmap_dump(hashmap* hh)
{
    printf("== hashmap dump ==\n");
    
    for (int i = 0; i < hh->cap; i++) {
	if (hh->data[i]) {
            print_pair(hh->data[i]);
	}
    }
}

