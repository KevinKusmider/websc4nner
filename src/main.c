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


int analyse(char *fileName);
int displayError(const char *error, MYSQL *mysql);
int bddConnect(MYSQL *mysql);

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int main (int argc, char **argv) {

 	MYSQL *mysql = mysql_init(NULL);//init
	MYSQL_RES *result = NULL;

	// Connect to db -> if failed display error
	if(!bddConnect(mysql)) return displayError(mysql_error(mysql), mysql);


	// Select & Display every elements
	mysql_query(mysql, "SELECT * FROM users"); // Make query
	result = mysql_use_result(mysql); // Store results
	// displaySqlResult(result); // Display results


	// Libération du jeu de resultat
	mysql_free_result(result);
	// Fermeture de mysql
	mysql_close(mysql);

	CURL *curl;
	CURLcode res;
	FILE *fp;

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if(curl) {
		fp = fopen("files/response.txt","wb");
		curl_easy_setopt(curl, CURLOPT_URL, "https://ges-cas.kordis.fr/login?service=https%3A%2F%2Fmyges.fr%2Fj_spring_cas_security_check");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "username='&password=454");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		 /*  Now specify the POST data */
   
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);

		/* Check for errors */
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));

		/* always cleanup */
		fclose(fp);
		curl_easy_cleanup(curl);
  	}

  	curl_global_cleanup();

	analyse("files/response.txt");


	/*GtkApplication *app;
	int status;

	app = gtk_application_new ("org.gtk.webscanner", G_APPLICATION_FLAGS_NONE);
  	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  	status = g_application_run (G_APPLICATION (app), argc, argv);
  	g_object_unref (app);

	return EXIT_SUCCESS;*/

	GtkWidget *fenetre_principale = NULL;
      GtkBuilder *builder = NULL;
      GError *error = NULL;
      gchar *filename = NULL;
      /* Initialisation de la librairie Gtk. */
      gtk_init(&argc, &argv);

      /* Ouverture du fichier Glade de la fenêtre principale */
      builder = gtk_builder_new();

      /* Création du chemin complet pour accéder au fichier test.glade. */
      /* g_build_filename(); construit le chemin complet en fonction du système */
      /* d'exploitation. ( / pour Linux et \ pour Windows) */
      filename =  g_build_filename ("test.glade", NULL);

          /* Chargement du fichier test.glade. */
      gtk_builder_add_from_file (builder, filename, &error);
      g_free (filename);
      if (error)
      {
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free (error);
        return code;
      }

        /* Récupération du pointeur de la fenêtre principale */
      fenetre_principale = GTK_WIDGET(gtk_builder_get_object (builder, "window_main"));

      /* Affectation du signal "destroy" à la fonction gtk_main_quit(); pour la */
      /* fermeture de la fenêtre. */
      g_signal_connect (G_OBJECT (fenetre_principale), "destroy", (GCallback)gtk_main_quit, NULL);

      /* Affichage de la fenêtre principale. */
      gtk_widget_show_all (fenetre_principale);

      gtk_main();

      return 0;
}

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
				printf("%s", line);
			}
		}
	}

	fclose(file);
	if (line)
        	free(line);

	return 1;
}
