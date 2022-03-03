/*** LIBRARIES ***/
// GCC Standards
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include<time.h>

// Imported libraries
#include <curl/curl.h>
#include <mysql.h>

// Own libraries
#include <global.h>
#include <functions.h>
#include <curl.h>
#include <main.h>
#include <database.h>


extern GLOBAL global;


/**
 * @brief Ecrit les ligne récupérées par curls dans le fichier mentioné
 * 
 * @param ptr Pointeur sur la ligne à écrire
 * @param size Taille de l'element
 * @param nmemb Nombre d'element
 * @param stream Fichier dans lequelle écrire
 * @return size_t 
 */
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}


/**
 * @brief Envoie une requête HTTP vers l'url mentioné
 * 
 * @param url URL vers laquelle envoyer la requete
 * @param postfield Valeur(s) a envoyer en POST
 * @return int 
 */
int send_curl(char *url, char * postfield) {
	CURL *curl = NULL;
	CURLcode res;
	FILE *file = NULL;

	// Initialiser les services SSL TLS
    if(curl_global_init(CURL_GLOBAL_ALL)) {
        if(config_check("debug", "true")) 
            fprintf(stderr, "\nImpossible d'initialiser curl_global_init.\n");
		return 0;
    }
	
	// Initiliaser curl
	if((curl = curl_easy_init()) == NULL) {
		if(config_check("debug", "true"))
			fprintf(stderr, "\nImpossible d'initialiser le pointeur curl\n");
		return 0;
	}

	// Ouvrir le fichier dans lequel sera écrit le code HTML
	if((file = fopen("files/response.txt","wb")) == NULL) {
		if(config_check("debug", "true"))
			fprintf(stderr, "\nImpossible d'ouvrir le ficher response.txt lors de l'envoi de la reqûete.\n");
		return 0;
	}

	// Vérifier que l'URL est non NULL
	if(url != NULL)
		curl_easy_setopt(curl, CURLOPT_URL, url);

	// S'il les postfields existe les envoyés avec la requete
	if(postfield != NULL) {
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfield);
	}

	// Spécifier le fichier a utiliser
	if(file != NULL)
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

	// Suivre les redirections
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	// Spécifier la function d'écriture
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

	// Executer la requete, res contiendra le code réponse
	res = curl_easy_perform(curl);

	// Vérification en cas d'erreurs 
	if(res != CURLE_OK) {
		fprintf(stderr, "\ncurl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		fclose(file);
		return 0;
	}

	/* CLEANING */
	fclose(file);

    curl_global_cleanup();
    if(curl != NULL) {
        curl_easy_cleanup(curl);
    }

	return 1;
}


/**
 * @brief Rechercher les informations de la target puis les écrire en bdd
 * 
 * @return int 
 */
int register_target() {
	char url[255];
	char name[255];
	char *ip = NULL;
	char *country = NULL;

	show_question("Menu > Websites > Add");

	// Demander le nom et l'url
	printf("\n  %-10s : ", "Name");
	scanf("%s", name);
	printf("  %-10s : ", "URL");
	scanf("%s", url);

	// Récuperer le HTML du site qui permet d'avoir les infos de l'url
	char inputName[255] = "url=";
	strcat(inputName,url);
	if(!send_curl("https://mon-adresse-ip.fr/trouver-une-adresse-ip/", inputName)) {
		printf("\nImpossible de récupérer les informations du site\n");
		return 0;
	}

	// Rechercher les informations nécessaires 
	// IP
	CHAR_ITEM *ips = NULL;
	search_lines_in_file("files/response.txt", "ipToCopy", &ips);
	if(ips != NULL){
		ip = find_content("\">","<", ips);
	}
	// Pays
	CHAR_ITEM *countries = NULL;
	search_lines_in_file("files/response.txt", "lead", &countries);
	if(countries != NULL){
		country = find_content(",","</", countries);
	}

	// Vérification des valeurs
	if(ip == NULL || country == NULL) {
		printf("\nImpossible de récupérer les informations de l'ip ou du pays\n");
		return 0;
	}

	printf("  %-10s : %s\n", "IP", ip);
	printf("  %-10s : %s\n", "Country", country);

	// Récupérer la date
	char * timeDisplay;
	int i;
	time_t tm;
    time(&tm);
	timeDisplay = ctime(&tm);

	// Enlever le retour à la ligne
	for(i=0; i< strlen(timeDisplay); i++) { 
		if(timeDisplay[i] == '\n')
			timeDisplay[i] = ' ';
	}

	// Ajouter la target à la bdd
	if(!add_target(name, url, ip, country, timeDisplay)) {
		fprintf(stderr, "\nImpossible d'ajouter la cible dans la base de données\n");
		return 0;
	}

	return 1;
}


/**
 * @brief Rechercher le contenu entre deux element dans une ligne
 * 
 * @param begin Debut de recherche
 * @param final Fin de rechercher
 * @param line Ligne dans laquelle rechercher
 * @return char* 
 */
char * find_content(char *begin, char *final, CHAR_ITEM *line) {
	char *target = NULL;
    char *start, *end;
	
	if ( start = strstr( line->value, begin ) )
    {
        start += strlen( begin );
        if ( end = strstr( start, final ) )
        {
            target = ( char * )malloc( end - start + 1 );
            memcpy( target, start, end - start );
            target[end - start] = '\0';
        }
    } else {
		return NULL;
	}

	/* CLEANING */
	list_clean_char_item(line);

    return target;
}

/**
 * @brief  Recherche chaque ligne dans un fichier contenant l'element find et les stoque en liste chainée 
 * 
 * @param fileName Nom du fichier dans lequel rechercher
 * @param find Valeur a rechercher
 * @param start Addresse pointeur qui pointera vers le premier item de la chaine
 * @return int 
 */
int search_lines_in_file(char *fileName, char *find, CHAR_ITEM **start) {
	FILE *file;
	char *line = NULL;
	size_t lineLen = 0;
	ssize_t read;

	// Ouverture du fichier
	if((file = fopen(fileName, "r")) == NULL) {
		if(config_check("debug", "true")) 
			fprintf(stderr, "\nImpossible d'ouvrir le fichier lors de search_lines_in_file\n");
		return 0;
	}

	// Recherche des lignes 
	while ((read = getline(&line, &lineLen, file)) != -1) {
		if(strstr(line, find) != NULL) {
			if(!trim(&line)) {
				if(config_check("debug", "true"))
					fprintf(stderr, "\nProbleme lors du trim dans search_lines_files\n");
			}
			list_add_char_item(start, "\0", line);
			line = NULL;
		}
	}

	/* CLEANING */
	fclose(file);
	if(line != NULL)
		free(line);

	return 1;
}


/**
 * @brief Récupère la valeur d'un attribut d'une ligne specifié par sa clé 
 * 
 * @param key Nom de l'attribut
 * @param attr Addresse de la chain de charactère qui contiendra la valeur de l'attribut
 * @param line Ligne dans la quelle chercher l'attribut
 * @return int 
 */
int get_attr_from_line(char *key, char **attr, char *line) {
	int i = 0, attrLength = 0; 
	char *attrStart = NULL, *attrEnd = NULL;

	// Verification des valeurs
	if(key == NULL || line == NULL) {
		if(config_check("debug", "true"))
			fprintf(stderr, "\nValeur(s) manquantes pour récupérer un attribut d'une ligne\n");
		return 0;
	}

	// Si la valeur de l'attribut n'est pas vide -> vider
	if(*attr != NULL) {
		free(*attr);
	}

	// Rechercher de la clé de l'attribut 
	if((attrStart = strstr(line, key)) == NULL) {
		*attr = NULL;
		if(config_check("debug", "true"))
			printf("\nAttribut %s de l'input non trouvé\n", key);
		return 0;
	}

	// Recherche du début de l'attribut 
	while(attrStart[i] != '"' && attrStart[i] != '\0') {
		i++;
		if(i>strlen(attrStart)+1) return 0;
	}
	if(attrStart[i] == '\0') return 0;
	attrStart = attrStart + i + 1;
	i = 0;

	// Recherche de la fin de l'attribut
	attrEnd = attrStart;
	while(attrEnd[i] != '"' && attrEnd[i] != '\0') {
		i++;
		if(i>strlen(attrEnd)+1) return 0;
	}
	if(attrEnd[i] == '\0') return 0;
	attrEnd = attrEnd + i;

	// Calcul de la longueur de l'attribut
	attrLength = (attrEnd - attrStart) / sizeof(char);
	i = 0;

	// Allouer la memoire pour stoquer l'attribut
	if((*attr = malloc(attrLength + sizeof(char))) == NULL ) {
		if(config_check("debug", "true"))
			fprintf(stderr, "\nImpossible d'allouer la mémoire get_attr_from_line\n");
		return 0;
	}

	// Pas de strcopy car la valeur na jamais été séparée
	while(i<(attrLength)) {
		(*attr)[i] = attrStart[i];
		++i;
	}
	(*attr)[i] = '\0';

	return 1;
}




