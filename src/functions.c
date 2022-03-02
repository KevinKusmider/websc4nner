#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <global.h>

void clean_stdin() {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

int trim(char **string) {
        int i = 0, j=0;
        size_t length = 0, newLength = 0;
        char *newString = NULL;

        if(!(length = strlen(*string))) {
            if(config_check("debug", "true")) 
                fprintf(stderr, "\nfunc trim : Longeur de chaine incalculable\n");
            return 0;
        }

        for(i=0; i<length; ++i) {
            if(!((*string)[i] == 32 || (*string)[i] == 13 || (*string)[i] == 9)) {
                newLength++;
            }
        }

        if(newLength == 0){
            if(config_check("debug", "true"))
                fprintf(stderr, "\nNouvelle longeur de cdc est vide lors de trim\n");
            return 0;
        } 
        newLength++; // For the \0

        if((newString = malloc(newLength * sizeof(char))) == NULL) {
            if(config_check("debug", "true"))
                fprintf(stderr, "\nImpossible d'initialiser le pointeur lors de trim\n");
            return 0;
        }

        for(i=0; i<length; ++i) {
            if(!((*string)[i] == 32 || (*string)[i] == '\n' || (*string)[i] == 9)) {
                newString[j] = (*string)[i];
                j++; 
            }
        }
        string[j] = '\0';

        free(*string);
        *string = newString;

        return 1;
}

void show_question(char * question) {
	printf("\n=====================================================================\n");
	printf("  %s\n", question);
	printf("=====================================================================\n");
}

int checkIntResponse(int value, int min, int max) {
    if(value >= min && value <= max) {
        return 1;
    } else {
        printf("Invalide !! La valeur doit etre comprise entre %d et %d.\n", min, max);
        return 0;
    }
}

int askForInt(char *question, int min, int max) {
    int value, response;

    do {
        printf("%s ", question);
        scanf("%d", &value);
        response = checkIntResponse(value, min, max);
        clean_stdin();
	} while(!response);

	return value;
}








