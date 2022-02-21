#include <stdio.h>
#include <stdlib.h>
#include <functions.h>
#include <regex.h>
#include <curl/curl.h>
#include <string.h>
#include <database.h>
#include <mysql.h>
int analyse(char *fileName);
void SaveData (char *url, char *inputCommand);

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

//Function that take a url in parameter & send a HTTP POST then return the html written in a .txt file to analyse it
int sendCurl(char *url, char *inputCommand) {
	CURL *curl;
	CURLcode res;
	FILE *fp;
	char inputName[255] ="username=";
	char inputName2[255] = "&password=454";
	strcat(inputName,inputCommand);
	strcat(inputName,inputName2);
	printf("\n%s\n", inputName);
	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if(curl) {
		fp = fopen("files/response.txt","wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
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
	SaveData(url,inputCommand);
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

void SaveData (char *url, char *inputCommand){
	MYSQL *mysql = mysql_init(NULL);
        MYSQL_RES *result = NULL;
	bddConnect(mysql);
	char sql_cmd[1000];
	char * addRequest= "INSERT INTO History (url, command) VALUES('";
	char temp[100]= "','";
	char finish[6]="')";
	//char finish[10]= "')\"";
	//strcat(addRequest, url);
	//strcat(addRequest, separate);
	//strcat(addRequest,inputCommand);
	//strcat(addRequest,finish);
	//printf("\n%s", addRequest);
	strcat(temp, inputCommand); 
	printf("\n%s\n",temp);
	strcat(temp,finish);
	printf("\n%s\n", temp);
	sprintf(sql_cmd,"%s%s%s", addRequest,url,temp);
	printf("%s", sql_cmd);
	// Select & Display every elements
          if(mysql_query(mysql, sql_cmd) !=0){
		fprintf(stderr, "Query Failure\n");
	   }					// Make query
           // result = mysql_use_result(mysql); // Store results
           // displaySqlResult(result); // Display results

            // Libération du jeu de resultat
           // mysql_free_result(result);
            // Fermeture de mysql
            mysql_close(mysql);

}
