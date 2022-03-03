/*** LIBRARIES ***/
// GCC Standars
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Own Libraries
#include <global.h>
#include <functions.h>
#include <curl.h>
#include <sqli.h>
#include <database.h>


extern GLOBAL global;


void injection_sql() {
	int option;
	char url[255];
    int i = 0, idForm = 0;
    int sqliChance = 0;

	show_question("Menu > Injection SQL");
	printf("\n 1. Automatique\n 2. Manuelle\n 0. Retour\n");

	switch(askForInt("\n===================#?:", 0, 2)) {
		case 0:
			return;
		
		case 1:
            break;

		case 2:
            // Selection de l'URL & Récupération code HTML
            if(global.url == NULL) { // Demander l'url si aucune n'a été selectionnée
                show_question("Menu > Injection SQL > Choix de l'URL");
			    printf("\n===================# URL ? ");
			    scanf("%s", url);
                printf("\n  Récupération du code HTML de L'URL/IP : %s\n", url);
                if(!send_curl(url, NULL)) break; // Envoi de la requete curl
            } else { // Sinon utiliser l'url selectionnée qui se trouve dans la struct global
                show_question("Menu > Injection SQL");
                printf("\n  Récupération du code HTML de L'URL/IP : %s\n", global.url);
                if(!send_curl(global.url, NULL)) break; // Envoi de la requete curl
            }

            // Récupération des formulaires et des inputs
            show_question("Menu > Injection SQL > Form(s)");
            FORM_ITEM * forms = NULL; // S'il y a des resultats forms contiendra l'addresse du premier FORM_ITEM
            if(get_forms("files/response.txt", &forms) && forms != NULL) {
                list_show_forms(forms); // Affichage des résultats
            } else {
                printf("\nAucun formulaire et aucun input n'ont été trouvés sur cette page.\n");
                // CLEAN FORMS
                break;
            }

            //list_clean_form_item(forms);
            //list_show_forms(forms); 

            
            // Selection du formulaire
            if(list_count_form_item(forms) > 1) {
                show_question("Menu > Injection SQL > Form(s) > Select");
                idForm = askForInt("\n===================# ID FORM ?", 1, list_count_form_item(forms));
                i=0;
                while(i < (idForm - 1)) {
                    forms = forms->next;
                    if(forms == NULL) {
                        if(config_check("debug", "true")) 
                            fprintf(stderr, "\nProblèmes lors de la selection du form\n");
                            // CLEAN FORM
                        break;
                    }
                    i++;
                } 
            }

            
            // Création des postfield envoyés avec la requete retour
            INPUT_ITEM *input = NULL;
            CHAR_ITEM *attr = NULL;
            char postfields[255] = "";

            if((input = forms->inputs) == NULL) {
                if(config_check("debug", "true"))
                    printf("\nAucun input dans le formulaire choisit\n");
                // CLEAN FORM
                break;
            }

            show_question("Menu > Injection SQL > Form(s) > Remplir valeurs");

            while(input != NULL) {
                if(input->attrs != NULL) {
                    attr = input->attrs;
                    while (attr != NULL) {
                        if(attr->key != NULL && !strcmp(attr->key, "name")) {
                            char valeur[255];
                            printf("\n  %-16s : %s", "Balise input", input->tag);
                            printf("\n  %-16s : %s", "Name", attr->value);
                            printf("\n  %-16s : ", "Valeur à envoyer");
                            scanf("%s", valeur);
                            strcat(postfields, attr->value);
                            strcat(postfields, "=");
                            strcat(postfields, valeur);
                        }
                        attr = attr->next;
                    }
                }
                if(input->next != NULL) {
                    strcat(postfields, "&");
                }
                input = input->next;
            } 

            printf("\n  %-16s : %s\n","Postfields", postfields);

            if(global.url != NULL) {
                send_curl(global.url, postfields);
                history_write ("SQLI", global.url, postfields);
            } else {
                history_write ("SQLI", global.url, postfields);
            }

            list_go_start_form_item(&forms); // Revenir au début de la liste au cas ou le formulaire choisi n'est pas le premier
            list_clean_form_item(forms); // Free les forms et leurs contenus

            CHAR_ITEM * error = NULL;
            CHAR_ITEM * syntax = NULL;
            CHAR_ITEM * sql = NULL;

            search_lines_in_file("files/response.txt", "ERROR", &error);
            search_lines_in_file("files/response.txt", "Error", &error);
            search_lines_in_file("files/response.txt", "error", &error);
            search_lines_in_file("files/response.txt", "Syntax", &syntax);
            search_lines_in_file("files/response.txt", "syntax", &syntax);
            search_lines_in_file("files/response.txt", "sql", &sql);
            search_lines_in_file("files/response.txt", "SQL", &sql);

            if(error != NULL) {
                sqliChance += 5;
                list_clean_char_item(error);
            }

            if(syntax != NULL) {
                sqliChance += 10;
                list_clean_char_item(syntax);
            }

            if(sql != NULL) {
                sqliChance += 10;
                printf("\n  Ligne trouvée : \n      %s\n", sql->value);
                list_clean_char_item(sql);
            }

            show_question("Menu > Injection SQL > Resultats");

            sqliChance =  (sqliChance*100) / 25;
            printf("\n  +---------------------------------------------------------------+");
            printf("\n  | %-61s |", "");
            printf("\n  | %-61s |", sqliChance ? "SQLI TROUVEE" : "SQLI NON DETECTEE");
            if(sqliChance) printf("\n  | Information sur à %3d%% %-38s |", sqliChance, "");
            printf("\n  | %-61s |", "");
            printf("\n  +---------------------------------------------------------------+\n");
			break;
	}
}


int get_forms(char *fileName, FORM_ITEM **forms) {
	FILE *file = NULL;
	char *line = NULL;
	size_t lineLen = 0;
	ssize_t read;

    fpos_t cursor;
    fpos_t lastLine;

    FORM_ITEM * form = NULL;
    INPUT_ITEM * input = NULL;

    int found = 0;
    int opened = 0;
    char car = '\0';
    char startTag[255];

    char * action = NULL;
    char * method = NULL;
    char * name = NULL;
    char * id = NULL;

	if((file = fopen(fileName, "r")) == NULL) {
		if(config_check("debug", "true")) 
			fprintf(stderr, "\nImpossible d'ouvrir le fichier lors de search_lines_in_file\n");
		return 0;
	}

	while ((read = getline(&line, &lineLen, file)) != -1) {
        
        if ( fgetpos(file, &cursor) != 0 ) {
            fprintf(stderr, "\nImpossible de récupérer la position de l'element dans le fichier\n");
            fclose(file);
            return 0;
        }
        
        // Tag ouvrant de form
		if(strstr(line, "<form") != NULL && !opened) { 
            if((form = malloc(sizeof(FORM_ITEM))) != NULL) {
                opened = 1;
                form->attrs = NULL;
                form->inputs = NULL;
                fsetpos(file, &lastLine);

                // Initialiser la valeur tag de la struct form
                if((form->tag = get_form_start_tag(file)) == NULL) {
                    fprintf(stderr, "\nImpossible d'initialise le tag form\n");
                } else {    // Si le tag est bien initialisé récupérer les attrs
                    action = NULL;
                    method = NULL;

                    if(get_attr_from_line("action=", &action, form->tag)) {
                        list_add_char_item(&(form->attrs), "action", action);
					}

                    if(get_attr_from_line("method=", &method, form->tag)) {
                        list_add_char_item(&(form->attrs), "method", method);
					}
                }

                fsetpos(file, &cursor);
            }
		}
        
		if(strstr(line, "<input") != NULL && opened && form != NULL) { // Chercher les INPUTS
            if((input = malloc(sizeof(INPUT_ITEM))) != NULL) {
                input->attrs = NULL;
                fsetpos(file, &lastLine);

                //printf("\n line : %s", line);
                if((input->tag = get_input_tag(file)) == NULL) { // Initialiser la valeur tag de la struct input
                    fprintf(stderr, "\nImpossible d'initialiser le tag input\n");
                } else {    // Si le tag est bien initialiser récupérer les attrs de linput
                    name = NULL;
                    id = NULL;

                    if(get_attr_from_line("name=", &name, input->tag)) {
                        list_add_char_item(&(input->attrs), "name\0", name);
					}
                }

                // AJouter l'input a la liste
                if(form->inputs != NULL ) {
                    form->inputs->prev = input;
                    input->next = form->inputs;
                } else {
                    input->next = NULL;
                }
                input->prev = NULL;

                form->inputs = input;
            }
            
            input = NULL;
		}
        
        if(strstr(line, "</form") != NULL) { // Tag fermant de form
            if(opened && form != NULL) {
                
                if(*forms != NULL ) {
                    (*forms)->prev = form;
                    form->next = *forms;
                } else {    
                    form->next = NULL;
                }
                form->prev = NULL;
                
                *forms = form;
                opened = 0;
            }
            form = NULL;
        }
        
        if ( fgetpos(file, &lastLine) != 0 ) { // Récupère la postion du curseur de la ligne actuelle
            fprintf(stderr, "\nImpossible de récupérer la position de l'element dans le fichier\n");
            fclose(file);
            return 0;
        }
        
	}
    
    if(file != NULL)
	    fclose(file);
	if(line != NULL)
		free(line);

	return 1;
}


char * get_form_start_tag(FILE * file) {
    fpos_t tagCursor;
    char car = '\0';
    int found = 0;
    int sti = 0;
    char startTag[255];
    char *res;

    while( !found && (((car=fgetc(file)) != EOF) && car != '>') && !fgetpos(file, &tagCursor)) {
        if(car == '<' && (car=fgetc(file) == 'f') && (car=fgetc(file) == 'o')) {
            found = 1;
        }
        fsetpos(file, &tagCursor);
    }

    if(car == EOF) {
        fclose(file);
        return NULL;
    }
    found = 0;

    sti = 0;
    while(((car=fgetc(file)) != EOF) && car != '>') {
        if(car != '\n') {
            startTag[sti] = car;
            sti++;
            startTag[sti] = '\0';
        }
    }

    if(car == EOF) {
        fclose(file);
        return NULL;
    }

    if((res = malloc(strlen(startTag) * sizeof(char) + 1)) == NULL) {
        if(config_check("debug", "true"))
            fprintf(stderr, "\nImpossible d'allouer la mémoire get_form_start_tag\n");
        return NULL;
    }

    strcpy(res, startTag);

    return res;
}

char * get_input_tag(FILE * file) {
    fpos_t tagCursor;
    char car = '\0';
    int found = 0;
    int sti = 0;
    char startTag[255];
    char *res;



    while( !found && !fgetpos(file, &tagCursor) && ((car=fgetc(file)) != EOF)) {
        if(car == '<' && (car=fgetc(file) == 'i') && (car=fgetc(file) == 'n') && (car=fgetc(file) == 'p')) {
            found = 1;
        } else {
            fsetpos(file, &tagCursor);
            car = fgetc(file);
            fgetpos(file, &tagCursor);
        }
        fsetpos(file, &tagCursor);
    }

    if(car == EOF) {
        fclose(file);
        return NULL;
    }
    found = 0;

    sti = 0;
    while(((car=fgetc(file)) != EOF) && car != '>') {
        if(car != '\n' && !(sti > 0 && startTag[sti-1] == ' ' && startTag[sti-1] == car)) {
            startTag[sti] = car;
            sti++;
            startTag[sti] = '\0';
        }
    }

    if(car == EOF) {
        fclose(file);
        return NULL;
    }

    if((res = malloc(strlen(startTag) * sizeof(char) + 1)) == NULL) {
        if(config_check("debug", "true"))
            fprintf(stderr, "\nImpossible d'allouer la mémoire get_input_tag\n");
        return NULL;
    }

    strcpy(res, startTag);

    return res;
}




            /*
			if(!trim(&line)) {
				if(config_check("debug", "true"))
					fprintf(stderr, "\nProbleme lors du trim dans search_lines_files\n");
			}
			list_add_char_item(start, "\0", line);
			line = NULL;
            */