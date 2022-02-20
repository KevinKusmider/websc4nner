#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Function that remove spaces
int trim(char *string) {
        int i = 0, j=0, length = 0;

        if(!(length = strlen(string))) {
            fprintf(stderr, "\nfunc trim : Longeur de chaine incalculable\n");
            return 0;
        }

        for(i=0; i<length; ++i) {
            if(!(string[i] == 32)) {
                string[j] = string[i];
                j++;
            }
        }
        string[j] = '\0';

        return 1;
}


