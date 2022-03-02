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
					break;
				case 4:
					history_result();
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
		interface_init(&argc, &argv);
	}

	return EXIT_SUCCESS;
}

int show_menu() {
	int option;
	show_question("Menu");
	printf("\n 1. Websistes\n 2. Injection SQL\n 3. Faille XSS\n 4. Historique\n 0. Stop program\n");

	option = askForInt("\n===================#?:", 0, 4);

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
			case 1:
				register_target();
				break;
			case 2:
				del_target();
				break;
			case 3:
				break;
		}
	}
}




