#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql/mysql.h>

#include <global.h>
#include <functions.h>

extern GLOBAL global;

int mysql_check_error() {
        if(global.mysql == NULL) {
                if(config_check("debug", "true")) {
                        fprintf(stderr, "\nImpossible de vérifier mysql error car mysql non initialisé\n");
                        return 1;
                }
        }

        if(strlen(mysql_error(global.mysql)) != 0) {
                fprintf(stderr, "\n%s\n", mysql_error(global.mysql));
                return 1;
        }

        return 0;
}

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

//Function that display all results in the database table
void displaySqlResult(MYSQL_RES *result) {
        MYSQL_ROW row;
        unsigned int i = 1;
        unsigned int num_champs = 0;

        num_champs = mysql_num_fields(result);
        printf("\nIl y a : %d champ(s) dans la requête SQL\n", num_champs);

        while((row = mysql_fetch_row(result))) {
                //On déclare un pointeur long non signé pour y stocker la taille des valeurs
                unsigned long *lengths;

                //On stocke ces tailles dans le pointeur
                lengths = mysql_fetch_lengths(result);

                //On fait une boucle pour avoir la valeur de chaque champs
                for(i = 0; i < num_champs; i++) {
                        //On ecrit toutes les valeurs
                        printf("[%.*s] ", (int) lengths[i], row[i] ? row[i] : "NULL");
                }
                printf("\n");
        }
}

void add_target (char *name, char *url, char *ip, char *country, char *timeDisplay) {
	char sql_cmd[2000];
	char * addRequest= "INSERT INTO targets (name, url, ip, country, creationDate) VALUES('";
	char temp[100]= "','";
	char finish[6]="')";
	//char creation[100]= "NULL";

	strcat(temp, url); 
	printf("\n%s\n",temp);
	strcat(temp, "','");
	strcat(temp, ip);
	strcat(temp, "','");
	strcat(temp, country);
	strcat(temp, "','");
	strcat(temp, timeDisplay);
	strcat(temp,finish);
	printf("\n%s\n", temp);
	sprintf(sql_cmd,"%s%s%s", addRequest,name,temp);
	printf("%s", sql_cmd);

	// Select & Display every elements
        if(mysql_query(global.mysql, sql_cmd) !=0) {
		fprintf(stderr, "Query Failure\n");
	}
	
	// result = mysql_use_result(mysql); // Store results
	// displaySqlResult(result); // Display results

	// Libération du jeu de resultat
	// mysql_free_result(result);
	// Fermeture de mysql
}

void del_target() {
	int id;
	char sql_cmd[1000];
	char * addRequest= "DELETE FROM targets WHERE id=";
	printf("Which URL you like to delete (select the id) : ");
	scanf("%d", &id);
	sprintf(sql_cmd,"%s%d", addRequest, id );
	if(mysql_query(global.mysql, sql_cmd) !=0){
		fprintf(stderr, "Query Failure\n");
	   }
	
}

int history_result() {
	// Select & Display every elements
        if(mysql_query(global.mysql, "SELECT * FROM history") != 0) {
                if(config_check("debug", "true"))
                        fprintf(stderr, "\nRequête impossible à executer\n");
                return 0;
        }
        if(global.result != NULL)
                mysql_free_result(global.result);

        global.result = mysql_use_result(global.mysql); // Store results
        displaySqlResult(global.result); // Display result
        // Libération du jeu de resultat
        mysql_free_result(global.result);

        return 1;
}

/*

        // Select & Display every elements
        mysql_query(mysql, "SELECT * FROM users"); // Make query
        result = mysql_use_result(mysql); // Store results
        // displaySqlResult(result); // Display results

        // Libération du jeu de resultat
        mysql_free_result(result);
        // Fermeture de mysql
        mysql_close(mysql);

*/
