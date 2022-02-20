#include <stdio.h>
#include <stdlib.h>
#include <functions.h>
#include <regex.h>
#include <curl/curl.h>

int analyse(char *fileName);

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

//Function that take a url in parameter & send a HTTP POST then return the html written in a .txt file to analyse it
int sendCurl(char *url) {
	CURL *curl;
	CURLcode res;
	FILE *fp;

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if(curl) {
		fp = fopen("files/response.txt","wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "username='&password=454");
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
                reti = regcomp(&regex, "<input", 0);
                if(!reti) {
                        reti = regexec(&regex, line, 0, NULL, 0);

                        if(!reti) {
                                printf("Retrieved line of length %zu:\n", read);
                                trim(line);
                                printf("%s\n", line);
                        }
                }
        }

        fclose(file);
        if (line)
                free(line);

        return 1;
}

/*int FindError (char *filename){
	FILE *file
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	regex_t regex;
	int reti;

	file = fopen(fileName, "r+b");
	if(file == NULL) exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, file)) != -1){
		reti = regcomp(&regex, "ERROR", 0);
		if(!reti) {

		if
*/
