// GCC Standards
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// Imported libraries
#include <curl/curl.h>
#include <mysql.h>

// Own libraries
#include <global.h>
#include <functions.h>

extern GLOBAL global;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
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

	return 1;
}

int sendCurl2(char *url) {
	CURL *curl;
	CURLcode res;
	FILE *fp;
	//char default="https://mon-adresse-ip.fr/trouver-une-adresse-ip/"
	char inputName[255] ="url=";
	strcat(inputName,url);
	printf("\n%s\n", inputName);
	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if(curl) {
		fp = fopen("files/response.txt","wb");
		curl_easy_setopt(curl, CURLOPT_URL, "https://mon-adresse-ip.fr/trouver-une-adresse-ip/");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, inputName);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		/*  Now specify the POST data */

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if(res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
		}

		/* always cleanup */
		fclose(fp);
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
	analyse("files/response.txt");
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


//Function that analyse the .txt file (the html returned code) and search for <input> via  regex
int analyse(char *fileName) {
        FILE *file;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        regex_t regex;
        int reti;

        file = fopen(fileName, "rb");
        if(file == NULL) exit(EXIT_FAILURE);

        while ((read = getline(&line, &len, file)) != -1) {
               // reti = regcomp(&regex, "<input", 0);
               // if(!reti) {
                       // reti = regexec(&regex, line, 0, NULL, 0);

                        //if(!reti) {
                               // printf("Retrieved line of length %zu:\n", read);
                                //trim(line);
                                printf("%s\n", line);
                       // }
               // }
        }

        fclose(file);
        if (line)
                free(line);

        return 1;
}

void SaveData (char *url, char *inputCommand) {
	char sql_cmd[1000];
	char * addRequest= "INSERT INTO History (url, command) VALUES('";
	char temp[100]= "','";
	char finish[6]="')";

	strcat(temp, inputCommand); 
	printf("\n%s\n",temp);
	strcat(temp,finish);
	printf("\n%s\n", temp);
	sprintf(sql_cmd,"%s%s%s", addRequest,url,temp);
	printf("%s", sql_cmd);
	// Select & Display every elements
    if(mysql_query(global.mysql, sql_cmd) !=0) {
		fprintf(stderr, "Query Failure\n");
	}					// Make query
	
	// result = mysql_use_result(mysql); // Store results
	// displaySqlResult(result); // Display results

	// Libération du jeu de resultat
	// mysql_free_result(result);
	// Fermeture de mysql
}
