/*** LIBRARIES ***/
// GCC Standards
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Imported Libraries
#include <mysql/mysql.h>

// Own Libraries
#include <global.h>
#include <functions.h>


extern GLOBAL global;


/**
 * @brief Vérifier si il y a une erreur mysql, affichage de celle-ci
 * 
 * @return int 
 */
int mysql_check_error() {
        if(global.mysql == NULL) {
                if(config_check("debug", "true")) {
                        fprintf(stderr, "\nImpossible de vérifier mysql error car mysql non initialisé\n");
                        return 0;
                }
        }

        if(strlen(mysql_error(global.mysql)) != 0) {
                fprintf(stderr, "\n%s\n", mysql_error(global.mysql));
                return 0;
        }

        return 1;
}


/**
 * @brief Connection a la Base de donnée, Les identifiants sont récuperés dans le fichier config
 * 
 * @param mysql 
 * @return int 
 */
int database_connect(MYSQL *mysql) {
        if( mysql == NULL ) return 0;

        mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

        if (!mysql_real_connect(mysql, config("db_host"), config("db_user"), config("db_password"), config("db_name"), 0, NULL, 0)) {
                if(config_check("debug", "true"))
                        fprintf(stderr, "\nImpossible de se conneccter à la bdd\n");
                return 0;
        } else {
                if(config_check("debug", "true"))
                        fprintf(stdout, "\nConnexion à la bdd réussi\n");
                return 1;
        }
}


/**
 * @brief Affiche tout les résultats d'une requete sql
 * 
 * @param result Resultat(s) retournés par la requete
 */
void displaySqlResult(MYSQL_RES *result) {
        MYSQL_ROW row;
        unsigned int i = 1;
        unsigned int num_champs = 0;  // Nombre de champs par ligne

        num_champs = mysql_num_fields(result);  // Nombre de champs par ligne
        printf("\nIl y a : %d champ(s) dans la requête SQL\n", num_champs);

        while((row = mysql_fetch_row(result))) {
                //On déclare un pointeur long non signé pour y stocker la taille des valeurs
                unsigned long *lengths;

                //On stocke ces tailles dans le pointeur
                lengths = mysql_fetch_lengths(result); // récupère la taille de toutes le colonnes dans la lignes actuelles

                // Boucler pour avoir la valeur de chaque champs
                for(i = 0; i < num_champs; i++) {
                        // Afficher toutes les valeurs
                        printf(" %.*s ", (int) lengths[i], row[i] ? row[i] : "NULL");
                }
                printf("\n");
        }
}


/**
 * @brief Ajout d'une target a la base de données
 * 
 * @param name 
 * @param url 
 * @param ip 
 * @param country 
 * @param timeDisplay 
 * @return int 
 */
int add_target (char *name, char *url, char *ip, char *country, char *timeDisplay) {
	char sql_cmd[2000];
	char * addRequest= "INSERT INTO targets (name, url, ip, country, creationDate) VALUES('";
	char temp[100]= "','";
	char finish[6]="')";

        // Création de la requete
	strcat(temp, url); 
	printf("\n%s\n",temp);
	strcat(temp, "','");
	strcat(temp, ip);
	strcat(temp, "','");
	strcat(temp, country);
	strcat(temp, "','");
	strcat(temp, timeDisplay);
	strcat(temp,finish);
	sprintf(sql_cmd,"%s%s%s", addRequest,name,temp);

	// Select & Display every elements
        if(mysql_query(global.mysql, sql_cmd) != 0) {
                fprintf(stderr, "\nQuery Failure\n");
                return 0;
	}

        return 1;
}


/**
 * @brief Suppresion d'une target de la base de donées
 * 
 * @return int 
 */
int del_target() {
	int id;
	char sql_cmd[255];
	char * addRequest= "DELETE FROM targets WHERE id=";

        show_question("Menu > Websites > Add");
        id = askForInt("\n  Which URL you like to select (select the id) : ", 0, 1000);
	sprintf(sql_cmd,"%s%d", addRequest, id );

	if(mysql_query(global.mysql, sql_cmd) !=0 ) {
		fprintf(stderr, "\nQuery Failure\n");
                return 0;
	}

        return 1;
}


/**
 * @brief Selection d'une target dans la bdd
 * 
 * @return int 
 */
int select_target() {
	int id;
	char sql_cmd[255];
	char * addRequest= "SELECT * FROM targets WHERE id=";
        char url[255];

        // Récupération ip de la target a selectionner
        show_question("Menu > Websites > Select");
        id = askForInt("\n  Which URL you would like to select (select the id) : ", 0, 1000);
	sprintf(sql_cmd,"%s%d", addRequest, id );

        // Envoye de la requete sql
	if(mysql_query(global.mysql, sql_cmd) !=0 ) {
		fprintf(stderr, "\nQuery Failure while select target\n");
                return 0;
	}
        
        // Récupération des resultats
        if(global.result != NULL) {
                mysql_free_result(global.result);
        }
        global.result = mysql_store_result(global.mysql); 

        // Vérifier s'il y a bien au moin une ligne
        if(!mysql_field_count(global.mysql)) {
                fprintf(stderr, "\nID non trouvé\n");
                return 0;
        }

        MYSQL_ROW row;
        unsigned int i = 1;
        unsigned int num_champs = 0; // Nombre de champs par ligne
        unsigned long *lengths;
        num_champs = mysql_num_fields(global.result);  // Nombre de champs par ligne
        if((row = mysql_fetch_row(global.result)) == NULL) {
		fprintf(stderr, "\nImpossible de recuperer le row lors du select target\n");
                if(global.result != NULL)
                        mysql_free_result(global.result);
                return 0;  
        } 

        // Récupérer la tailles de toutes le colonnes dans la lignes actuelles
        lengths = mysql_fetch_lengths(global.result);

        for(i = 0; i < num_champs; i++) {
                if(i == 2) {
                        sprintf(url, "%.*s", (int) lengths[i], row[i] ? row[i] : "NULL");
                }
        }

        // Verification de l'URL
        if(url == NULL || !strcasecmp(url, "NULL")) {
                return 0;
        }

        // Libération de l'ancienne valeur
        if(global.url != NULL ) {
                free(global.url);
        }

        if((global.url = malloc(sizeof(char) * strlen(url) + 1)) == NULL) {
                fprintf(stderr, "\nImpossible d'allouer la mémoire lors de la selection de l'url\n");
                return 0;
        }
        strcpy(global.url, url);

        /* CLEANING */
        // Libération du jeu de resultat
        mysql_free_result(global.result);

        return 1;
}

/**
 * @brief Ecrire en base de donnée les informations concernant un test;
 * 
 * @param url 
 * @param inputCommand 
 */
void history_write(char *type, char *url, char *inputCommand) {
	char sql_cmd[1000];
	char * addRequest= "INSERT INTO history (type, url, command) VALUES('";
	char temp[100]= "','";
	char finish[6]="')";

        int i = 0;
        while(inputCommand[i] != '\0') {
                if(inputCommand[i] == '\'') {
                        inputCommand[i] = '"';
                }
                i++;
        }

	sprintf(sql_cmd,"%s%s%s%s%s%s%s", addRequest,type, temp, url, temp, inputCommand, finish);

	// Select & Display every elements
        if(mysql_query(global.mysql, sql_cmd) !=0) {
		fprintf(stderr, "Query Failure\n");
	}
	
}


/**
 * @brief Afficher l'historique des tests
 * 
 * @return int 
 */
int history_result() {
	MYSQL_FIELD *fields;
	MYSQL_ROW row;
	size_t i = 0;
	size_t num_champs = 0;
	int flengths[6] = {3,20,55,66};

        show_question("Menu > History");

	// Execution requete sql
	if(0 != mysql_query(global.mysql, "SELECT * FROM history")) {
                if(config_check("debug", "true"))
                        fprintf(stderr, "\nRequête impossible à executer\n");
                return 0;
        }

	// Récupération des données
	global.result = mysql_store_result(global.mysql);
	num_champs = mysql_num_fields(global.result); // Nombre de champs par ligne
	fields = mysql_fetch_fields(global.result); // Récupérer le nom des colonnes

	// Afficher les titres des colonnes
	printf("\n+-----+----------------------+---------------------------------------------------------+---------------------------------------------------------------------+\n");
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
	printf("+-----+----------------------+---------------------------------------------------------+---------------------------------------------------------------------+\n");

	// CLEANING
	mysql_free_result(global.result);
	global.result = NULL;


        return 1;
}