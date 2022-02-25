// GCC Standards
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// Imported libraries
#include <curl/curl.h>

// Own libraries
#include <global.h>
#include <functions.h>

extern GLOBAL global;

int analyse(char *fileName);

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
	printf("\nwriteddate: %s", ptr);
    return written;
}

int send_curl(char *url, char * postfield) {
	CURL *curl = NULL;
	CURLcode res;
	FILE *file = NULL;

    if(curl_global_init(CURL_GLOBAL_ALL)) {
        if(config_check("debug", "true")) 
            fprintf(stderr, "\nImpossible d'initialiser curl_global_init.\n");
    }
	
	if((curl = curl_easy_init()) == NULL) {
		if(config_check("debug", "true"))
			fprintf(stderr, "\nImpossible d'initialiser le pointeur curl\n");
		return 0;
	}

	if((file = fopen("files/response.txt","wb")) == NULL) {
		if(config_check("debug", "true"))
			fprintf(stderr, "\nImpossible d'ouvrir le ficher responses.txt lors de l'envoi de la reqûete.\n");
		return 0;
	}

	if(url != NULL)
		curl_easy_setopt(curl, CURLOPT_URL, url);

	if(postfield != NULL) {
		printf("\npostfield in sendcurl %s\n", postfield);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfield);
	}

	if(file != NULL)
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);


	/* Perform the request, res will get the return code */
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK && config_check("debug", "true")) {
		fprintf(stderr, "\ncurl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	}

	/* always cleanup */
	fclose(file);

    curl_global_cleanup();
    if(curl != NULL) {
        curl_easy_cleanup(curl);
    }
}

int search_lines_in_file(char *fileName, char *find, CHAR_ITEM **start) {
	FILE *file;
	char *line = NULL;
	size_t lineLen = 0;
	ssize_t read;

	if((file = fopen(fileName, "r")) == NULL) {
		if(config_check("debug", "true")) 
			fprintf(stderr, "\nImpossible d'ouvrir le fichier lors de search_lines_in_file\n");
		return 0;
	}

	while ((read = getline(&line, &lineLen, file)) != -1) {
		if(strstr(line, find) != NULL) {
			if(!trim(&line)) {
				if(config_check("debug", "true"))
					fprintf(stderr, "\nProbleme lors du trim dans search_lines_files\n");
			}
			list_add_char_item(start, "\0", line);
			line = NULL;
		}
	}

	fclose(file);
	if(line != NULL)
		free(line);

	return 1;
}

int get_attr_from_line(char *key, char **attr, char *line) {
	int i = 0, attrLength = 0; 
	char *attrStart = NULL, *attrEnd = NULL;

	if(key == NULL || line == NULL) {
		if(config_check("debug", "true"))
			fprintf(stderr, "\nValeur(s) manquantes pour récupérer un attribut d'une ligne\n");
		return 0;
	}

	if(*attr != NULL) {
		free(*attr);
	}

	if((attrStart = strstr(line, key)) == NULL) {
		return 0;
	}

	while(attrStart[i] != '"' && attrStart[i] != '\0') {
		i++;
		if(i>strlen(attrStart)+1) return 0;
	}
	if(attrStart[i] == '\0') return 0;
	attrStart = attrStart + i + 1;
	i = 0;

	attrEnd = attrStart;
	while(attrEnd[i] != '"' && attrEnd[i] != '\0') {
		i++;
		if(i>strlen(attrEnd)+1) return 0;
	}
	if(attrEnd[i] == '\0') return 0;
	attrEnd = attrEnd + i;

	attrLength = (attrEnd - attrStart) / sizeof(char);
	i = 0;

	if((*attr = malloc(attrLength + sizeof(char))) == NULL ) {
		if(config_check("debug", "true"))
			fprintf(stderr, "\nImpossible d'allouer la mémoire get_attr_from_line\n");
		return 0;
	}

	while(i<(attrLength)) {
		(*attr)[i] = attrStart[i];
		++i;
	}
	(*attr)[i] = '\0';

	return 1;
}
