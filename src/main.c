/*** LIBRARIES ***/
// GCC Standards
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Imported Libraries
#include <mysql/mysql.h>
#include <gtk/gtk.h>
#include <curl/curl.h>

// Own Libraries
#include <database.h>
#include <interface.h>
#include <functions.h>
#include <curl.h>
#include <global.h>
#include <main.h>
#include <sqli.h>
#include <xss.h>


extern GLOBAL global;

/**
 * @brief MAIN FUNCTION
 * 
 * @param argc Nombre de paramètres lors de l'éxécution
 * @param argv Valeurs des paramètres
 * @return int 
 */
int main (int argc, char **argv) {
	int response, option, program;

	// Initialiser la structures globale
	init_global();

	// Affichage du titre
	printf("\n\ndP   dP   dP          dP       .d88888b           dP   dP\n88   88   88          88       88.    '          88   88\n88  .8P  .8P .d8888b. 88d888b. `Y88888b. .d8888b. 88aaa88 88d888b. 88d888b. .d8888b. 88d888b.\n88  d8'  d8' 88ooood8 88'  `88       `8b 88'  `      88 88'  `88 88'  `88 88ooood8 88'  `88\n88.d8P8.d8P  88.  ... 88.  .88 d8'   .8P 88.  ...      88 88    88 88    88 88.  ... 88\n8888' Y88'   `88888P' 88Y8888'  Y88888P  `88888P'      dP dP    dP dP    dP `88888P' dP\noooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");

	// Choix entre console mode et interface
	show_question("What would you like to do ?");
	printf("\n 1. Console mode\n 2. Interface mode\n 0. Stop program\n");
	option = askForInt("\n===================#?:", 0, 2);

	// Arreter le programme
	if(option == 0) { 
		global_clean();
		exit(EXIT_SUCCESS); 
	}

	// Console mode 
	if(option == 1) {
		while(1) { // Tant que l'utilisateur ne souhaite pas quitter le programme
			switch (show_menu()) { // Afficher le menu et demander l'action a effectuer
				case 1:
					websites(); // Affichage et selection des targets
					break;
				case 2:
					injection_sql(); // Injection SQL
					break;
				case 3:
					xss(); // Xss
					break;
				case 4:
					history_result(); // Affichage de l'historique des attaques
					break;
				case 0: // Arreter le programme
					global_clean();
					exit(EXIT_SUCCESS);
					break;
				default: // Normalement impossible
					fprintf(stderr, "\nShow menu answer not found\n");
			}
		}
	}

	// Lancement aplication en mode interface
	if(option == 2) {
		interface_init(&argc, &argv);
	}

	global_clean();
	return EXIT_SUCCESS;
}


/**
 * @brief Affiche le menu et retourne la valeur entré par l'utisateur
 * 
 * @return int 
 */
int show_menu() {
	int option;
	show_question("Menu");
	printf("\n 1. Websistes\n 2. Injection SQL\n 3. Faille XSS\n 4. Historique\n 0. Stop program\n");

	option = askForInt("\n===================#?:", 0, 4); // demande un int entre 0 et 4 et affiche la question

	return option;
}


/**
 * @brief Récupaération des données des targets dans la bdd et Affichages  
 * 
 */
void show_websites() {
	MYSQL_FIELD *fields;
	MYSQL_ROW row;
	size_t i = 0;
	size_t num_champs = 0;
	int flengths[6] = {3,20,55,20,15,25};

	// Execution requete sql
	if(0 != mysql_query(global.mysql, "SELECT * FROM targets")) return;

	// Récupération des données
	global.result = mysql_store_result(global.mysql);
	num_champs = mysql_num_fields(global.result); // Nombre de champs par ligne
	fields = mysql_fetch_fields(global.result); // Récupérer le nom des colonnes

	// Afficher les titres des colonnes
	printf("\n+-----+----------------------+---------------------------------------------------------+----------------------+-----------------+----------------------------+\n");
	for(i=0 ; i<num_champs ; i++) {
		printf("| %-*s %s", flengths[i], fields[i].name, i == num_champs-1 ? " |" : "");
	}
	printf("\n");

	// Afficher les valeurs des colonnes
	while((row = mysql_fetch_row(global.result))) {
		size_t *lengths; // Array qui contiendra la taille de chaque champs
		lengths = mysql_fetch_lengths(global.result); // récupère la taille de toutes le colonnes dans la lignes actuelles

		for(i = 0; i < num_champs; i++) {
				printf("| %-*s %s", flengths[i], row[i] ? row[i] : "NULL", i == num_champs-1 ? " |" : "");
		}
		printf("\n");
	}
	printf("+-----+----------------------+---------------------------------------------------------+----------------------+-----------------+----------------------------+\n");

	// CLEANING
	mysql_free_result(global.result);
	global.result = NULL;
}


/**
 * @brief Boucle sur le menu des targets tant que l'utilisateur ne souhaite pas sortir
 * 
 */
void websites() {
	while(1) {
		show_question("Menu > Websites");
		printf("\nSelected URL : %s\n", global.url != NULL ? global.url : "NONE"); // Affichage target stocké dans la struct global si elle existe.
		show_websites();  // Afficher targets
		printf("\n 1. Ajouter\n 2. Supprimer\n 3. Sélectionner\n 0. Retour\n");

		switch (askForInt("\n===================#?:", 0, 3)) {
			case 0:
				return;
			case 1:
				register_target(); // Ajouter target
				break;
			case 2:
				del_target(); // Supprimer target
				break;
			case 3:
				select_target(); // Selectionner target
				break;
		}
	}
}




