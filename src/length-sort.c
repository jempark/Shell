#include <stdio.h>
#include <string.h>

#include "svec.h"

void
sort(svec* xs)
{
    for (int i = 0; i < xs->size; i++) {
	if (i != 0 && !(strlen(svec_get(xs, i)) >= strlen(svec_get(xs, i - 1)))) {
	   svec_swap(xs, i, i - 1);
	   i -= 2;
	}
    }
}

void
chomp(char* text)
{
     strtok(text, "\n");
}

int
main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage:\n  %s input-file\n", argv[0]);
        return 1;
    }

    FILE* fh = fopen(argv[1], "r");
    if (!fh) {
        perror("open failed");
        return 1;
    }

    svec* xs = make_svec();
    char temp[128];

    while (1) {
        char* line = fgets(temp, 128, fh);
        if (!line) {
            break;
        }

        chomp(line);
        svec_push_back(xs, line);
    }

    fclose(fh);

    sort(xs);

    for (int ii = 0; ii < xs->size; ++ii) {
        char* line = svec_get(xs, ii);
        printf("%s\n", line);
    }

    free_svec(xs);
    return 0;
}
