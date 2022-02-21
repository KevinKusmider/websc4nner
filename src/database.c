#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>


//Function that return error when we cant connect to mysql
int displayError(const char *error, MYSQL *mysql) {
        if(strlen(error) != 0) fprintf(stderr, "\n%s\n", error);
        else fprintf(stderr, "\nUne erreur s'est produite\n");

        if(mysql != NULL) mysql_close(mysql);

        return 1;
}
//Function that let us connect to database
int bddConnect(MYSQL *mysql) {
        if( mysql == NULL ) return 1;

        mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

        if (!mysql_real_connect(mysql, "localhost", "kusmider", "Secure11", "test", 0, NULL, 0)) {
                printf("Impossible de se conneccter à la bdd\n");
                return 0;
        } else {
                printf("Connexion à la bdd réussi\n");
                return 1;
        }
}

//Function that display all results in the database table
void displaySqlResult(MYSQL_RES *result) {
        MYSQL_ROW row;
        unsigned int i = 1;
        unsigned int num_champs = 0;

        num_champs = mysql_num_fields(result);
        printf("Il y a : %d champ(s) dans la requête SQL\n", num_champs);

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

void historyResult(){
	MYSQL *mysql = mysql_init(NULL);
        MYSQL_RES *result = NULL;
        bddConnect(mysql);
	// Select & Display every elements
        mysql_query(mysql, "SELECT * FROM History"); // Make query
        result = mysql_use_result(mysql); // Store results
        displaySqlResult(result); // Display result
        // Libération du jeu de resultat
        mysql_free_result(result);
        // Fermeture de mysql
        mysql_close(mysql);
}
