#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <mysql/mysql.h>
#include <gtk/gtk.h>
#include <curl/curl.h>

#include <database.h>
#include <interface.h>
#include <functions.h>
#include <curl.h>
#include <global.h>
#include <main.h>
#include <sqli.h>


extern GLOBAL global;


int main (int argc, char **argv) {
	int response, option, program;

	init_global();

	printf("\n\ndP   dP   dP          dP       .d88888b           dP   dP\n88   88   88          88       88.    '          88   88\n88  .8P  .8P .d8888b. 88d888b. `Y88888b. .d8888b. 88aaa88 88d888b. 88d888b. .d8888b. 88d888b.\n88  d8'  d8' 88ooood8 88'  `88       `8b 88'  `      88 88'  `88 88'  `88 88ooood8 88'  `88\n88.d8P8.d8P  88.  ... 88.  .88 d8'   .8P 88.  ...      88 88    88 88    88 88.  ... 88\n8888' Y88'   `88888P' 88Y8888'  Y88888P  `88888P'      dP dP    dP dP    dP `88888P' dP\noooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");

	show_question("What would you like to do ?");
	printf("\n 1. Console mode\n 2. Interface mode\n 0. Stop program\n");
	option = askForInt("\n===================#?:", 0, 2);

	if(option == 0) { 
		// DONT FORGET TO CLEAN
		exit(EXIT_SUCCESS); 
	}

	if(option == 1) {
		while(1) {
			switch (show_menu()) {
				case 1:
					websites();
					break;
				case 2:
					injection_sql();
					break;
				case 3:
					xss2();
				case 5:
					historyResult();
					break;
				case 0:
					// APPELLER FUNC FREE GLOBAL
					exit(EXIT_SUCCESS);
					break;
				default:
					printf("Show menu answer not found");
			}
		}
	}

	if(option == 2) {
		GtkApplication *app;
		int status;

		app = gtk_application_new ("org.gtk.webscanner", G_APPLICATION_FLAGS_NONE);
  		g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	  	status = g_application_run (G_APPLICATION (app), argc, argv);
  		g_object_unref (app);
	}

	return EXIT_SUCCESS;
}

int show_menu() {
	int option;
	show_question("Menu");
	printf("\n 1. Websistes\n 2. Injection SQL\n 3. Faille XSS\n 4. Test general\n 5. Historique\n 0. Stop program\n");

	option = askForInt("\n===================#?:", 0, 5);

	return option;
}


void show_websites() {
	MYSQL_FIELD *fields;
	MYSQL_ROW row;
	size_t i = 0;
	size_t num_champs = 0;
	int flengths[6] = {3,20,55,20,15,15};

	mysql_query(global.mysql, "SELECT * FROM targets");
	global.result = mysql_store_result(global.mysql);
	
	num_champs = mysql_num_fields(global.result);
	fields = mysql_fetch_fields(global.result);

	// Afficher les titres des colonnes
	printf("\n");
	for(i=0 ; i<num_champs ; i++) {
		printf("| %-*s ", flengths[i], fields[i].name);
	}
	printf("\n");

	// Afficher les valeurs des colonnes
	while((row = mysql_fetch_row(global.result))) {
		size_t *lengths; // Array qui contiendra la taille de chaque fields
		lengths = mysql_fetch_lengths(global.result);

		for(i = 0; i < num_champs; i++) {
				printf("| %-*s ", flengths[i], row[i] ? row[i] : "NULL");
		}
		printf("\n");
	}
	mysql_free_result(global.result);
}

void websites() {
	while(1) {
		show_question("Menu > Websites");
		show_websites();
		printf("\n 1. Ajouter\n 2. Supprimer\n 3. Sélectionner\n 0. Retour\n");

		switch (askForInt("\n===================#?:", 0, 3)) {
			case 0:
				return;
		}
	}
}

void addUrl() {
	char url[255];
	char inputCommand[255];
	char sql_cmd[1000];
	printf("url ? ");
	scanf("%s", url);
	char * addRequest= "INSERT INTO Websites (url) VALUES('";
	char finish[6]="')";
	sprintf(sql_cmd,"%s%s%s", addRequest, url, finish );
	if(mysql_query(global.mysql, sql_cmd) !=0){
		fprintf(stderr, "Query Failure\n");
	   }
}

void showUrl() {
	// Select & Display every elements
    mysql_query(global.mysql, "SELECT * FROM Websites"); // Make query
    global.result = mysql_use_result(global.mysql); // Store results
    displaySqlResult(global.result); // Display result
    // Libération du jeu de resultat
    mysql_free_result(global.result);
}

void deleteUrl() {
	int id;
	char sql_cmd[1000];
	char * addRequest= "DELETE FROM Websites WHERE id=";
	printf("Which URL you like to delete (select the id) : ");
	scanf("%d", &id);
	sprintf(sql_cmd,"%s%d", addRequest, id );
	if(mysql_query(global.mysql, sql_cmd) !=0){
		fprintf(stderr, "Query Failure\n");
	   }
	
}

//Function that capture the URL + send it to Curl via sendCurl() function
void xss2() {
	char url[255];
	printf("url ? ");
	scanf("%s", url);
	sendCurl2(url);
}

