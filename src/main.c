#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <string.h>
#include <regex.h>
#include <database.h>
#include <interface.h>
#include <functions.h>
#include <curl.h>

int analyse(char *fileName);
int displayError(const char *error, MYSQL *mysql);
int bddConnect(MYSQL *mysql);

//Function to verifie the input for menu
int checkResponse(int value, int min, int max) {
	if(value >= min && value <= max) {
		return 1;
	}

	printf("Choice not found motherfucker\n");
	return 0;
}

//Function to display the question
void showQuestion(char * question) {
	printf("=====================================================================\n");
	printf("  %s\n", question);
	printf("=====================================================================\n\n");
}

//Function to display the menu + verifie input with a do while loop
int showMenu() {
	int option, response;
	showQuestion("Menu");
	printf(" 1. Websistes\n 2. Injection SQL\n 3. Attaque DOS\n 4. Test general\n 5. Historique\n 6. Stop program\n\n");

	do {
		printf("===================#?:");
        	scanf("%d", &option);
		response = checkResponse(option , 1, 6);
	} while(!response);

	return option;
}

//Function that capture the URL + send it to Curl via sendCurl() function
void xss() {
	char url[255];
	printf("url ? ");
	scanf("%s", url);

	sendCurl(url);
}

int main (int argc, char **argv) {

	int response, option, program;

	printf("dP   dP   dP          dP       .d88888b           dP   dP\n88   88   88          88       88.    '          88   88\n88  .8P  .8P .d8888b. 88d888b. `Y88888b. .d8888b. 88aaa88 88d888b. 88d888b. .d8888b. 88d888b.\n88  d8'  d8' 88ooood8 88'  `88       `8b 88'  `      88 88'  `88 88'  `88 88ooood8 88'  `88\n88.d8P8.d8P  88.  ... 88.  .88 d8'   .8P 88.  ...      88 88    88 88    88 88.  ... 88\n8888' Y88'   `88888P' 88Y8888'  Y88888P  `88888P'      dP dP    dP dP    dP `88888P' dP\noooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n\n");


	showQuestion("What would you like to do ?");
	printf(" 1. Console mode\n 2. Interface mode\n 3. Stop program\n\n");

	do {
		printf("===================#?:");
        	scanf("%d", &option);
		response = checkResponse(option , 1, 3);
	} while(!response);

	if(option == 3) { exit(EXIT_SUCCESS); }

        MYSQL *mysql = mysql_init(NULL);
        MYSQL_RES *result = NULL;

	// Connect to db -> if failed display error
	if(!bddConnect(mysql)) { return displayError(mysql_error(mysql), mysql); }

	if(option == 1) {

		if(1) {
			while(1) {
				switch (showMenu()) {
					case 1:
						break;
					case 2:
						xss();
						break;
					case 6:
						exit(EXIT_SUCCESS);
						break;
					default:
						printf("Show menu answer not found");
				}
			}
		} else {
			// Select & Display every elements
			mysql_query(mysql, "SELECT * FROM users"); // Make query
			result = mysql_use_result(mysql); // Store results
			// displaySqlResult(result); // Display results

			// Libération du jeu de resultat
			mysql_free_result(result);
			// Fermeture de mysql
			mysql_close(mysql);
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

