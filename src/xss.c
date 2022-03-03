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


void xss() {
	int option;
	char url[255];
    int i = 0, idForm = 0;
    int xssChance = 0;

	show_question("Menu > XSS");

    // Selection de l'URL & Récupération code HTML
    if(global.url == NULL) { // Demander l'url si aucune n'a été selectionnée
        show_question("Menu > XSS > Choix de l'URL");
        printf("\n===================# URL ? ");
        scanf("%s", url);
        printf("\n  Récupération du code HTML de L'URL/IP : %s\n", url);
        if(!send_curl(url, NULL)) return;
    } else { // Sinon utiliser l'url selectionnée qui se trouve dans la struct global
        show_question("Menu > XSS");
        printf("\n  Récupération du code HTML de L'URL/IP : %s\n", global.url);
        send_curl(global.url, NULL); // Envoi de la requete curl
    }

    // Récupération des formulaires et des inputs
    show_question("Menu > XSS > Form(s)");
    FORM_ITEM * forms = NULL; // S'il y a des resultats forms contiendra l'addresse du premier FORM_ITEM
    if(get_forms("files/response.txt", &forms) && forms != NULL) {
        list_show_forms(forms); // Affichage des résultats
    } else {
        printf("\nAucun formulaire et aucun input n'ont été trouvés sur cette page.\n");
        list_clean_form_item(forms);
        return;
    }
    
    // Selection du formulaire
    if(list_count_form_item(forms) > 1) {
        show_question("Menu > XSS > Form(s) > Select");
        idForm = askForInt("\n===================# ID FORM ?", 1, list_count_form_item(forms));
        i=0;
        while(i < (idForm - 1)) {
            forms = forms->next;
            if(forms == NULL) {
                if(config_check("debug", "true")) 
                    fprintf(stderr, "\nProblèmes lors de la selection du form\n");
                    list_clean_form_item(forms);
                return;
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
        return;
    }

    show_question("Menu > XSS > Form(s) > Fill values");

    char chooseAttr[255];
    printf("\n  Choose name : ");
    scanf("%s", chooseAttr);

    while(input != NULL) {
        if(input->attrs != NULL) {
            attr = input->attrs;
            while (attr != NULL) {
                if(attr->key != NULL && !strcmp(attr->key, "name")) {
                    char valeur[255];
                    printf("\n  %-16s : %s", "Balise input", input->tag);
                    printf("\n  %-16s : %s", "Name", attr->value);
                    printf("\n  %-16s : ", "Valeur à envoyer");
                    strcat(postfields, attr->value);
                    strcat(postfields, "=");
                    if (!strcmp(attr->value, chooseAttr)) {
                        strcat(postfields, "<script>let body = document.getElementsByTagName('body')[0];let el = document.createElement('p');el.innerHTML = 'TOKENHACKEDXSS';body.append(el);</script>");
                    }
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
        history_write ("XSS", global.url, postfields);
    } else {
        send_curl(url, postfields);
        history_write("XSS", url, postfields);
    }

    list_go_start_form_item(&forms); // Revenir au début de la liste au cas ou le formulaire choisi n'est pas le premier
    list_clean_form_item(forms); // Free les forms et leurs contenus

    CHAR_ITEM * error = NULL;

    search_lines_in_file("files/response.txt", "TOKENHACKEDXSS", &error);

    if(error != NULL) {
        xssChance += 5;
        list_clean_char_item(error);
    }

    show_question("Menu > XSS > Results");

    printf("\n  +---------------------------------------------------------------+");
    printf("\n  | %-61s |", "");
    printf("\n  | %-61s |", xssChance ? "XSS DETECTED" : "XSS NOT DETECTEE");
    printf("\n  | %-61s |", "");
    printf("\n  +---------------------------------------------------------------+\n");

    return;
}
