#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <pam_appl.h>
#include <pam_misc.h>
#include <string.h>
#include <inttypes.h>
#include "conv.h"

/** Fonction qui affiche un message d'erreur en fonction du code de retour
 *
 */
void show_error(char * prefix, int code) {
    fprintf(stderr, "%s: ", prefix);
    switch (code) { // Affiche le message correspondant au code d'erreur.
    case PAM_USER_UNKNOWN:
        fprintf(stderr, "Utilisateur inconnu.");
        break;
    case PAM_TRY_AGAIN:
        fprintf(stderr, "L'un des modules a échoué.");
        break;
    case PAM_PERM_DENIED:
        fprintf(stderr, "Accès refusé.");
        break;
    case PAM_AUTHTOK_ERR:
        fprintf(stderr, "Impossible d'obtenir le nouveau mot de passe.");
        break;
    case PAM_AUTHTOK_RECOVERY_ERR:
        fprintf(stderr, "Mot de passe erroné.");
        break;
    case PAM_SYSTEM_ERR:
        fprintf(stderr, "Erreur système.");
        break;
    case PAM_CONV_ERR:
        fprintf(stderr, "Erreur lors de la conversation avec le module PAM");
        break;
    default:
        fprintf(stderr, "Erreur inconnue: %d.", code);
        break;
    }
    fprintf(stderr, "\n");
}

void cleanup(pam_handle_t * pamh, void * data, int error_status) {
    free(data);
}

void parsing_error(char * prgname) {
    fprintf(stderr, "utilisation: ");
    fprintf(stderr, "%s [-l login] [-m (hotp|totp)] [-s 16-8] [-q quantum]\n", 
            prgname);
    exit(EXIT_FAILURE);
}

int main(int argc, char * argv[]) {
    char * user = NULL; /** Le nom de l'utilisateur */
    conv_data app_data;
    memset(app_data.method, 0, 5);
    app_data.length = -1;
    app_data.quantum = - 1;
    int retval; /** Stockage de valeur de retour. */

    // TODO: Parser argv
    char arg;
    while ((arg = getopt(argc, argv, "l:m:s:q:")) != -1) {
        switch (arg) {
        case 'l':
            if (optarg[0] != '-') {
                user = strdup(optarg);
            } else {
                parsing_error(argv[0]);
            }
            break;
        case 'm':
            if (optarg[0] != '-') {
                memcpy(app_data.method, optarg, 4);
                app_data.method[4] = 0;
            } else {
                parsing_error(argv[0]);
            }
            break;
        case 's':
            if (optarg[0] != '-') {
                char * endptr;
                app_data.length = strtol(optarg, &endptr, 10);
                if (*endptr != 0) {
                    parsing_error(argv[0]);
                }
            } else {
                parsing_error(argv[0]);
            }
            break;
        case 'q':
            if (optarg[0] != '-') {
                char * endptr;
                app_data.quantum = strtol(optarg, &endptr, 10);
                if (*endptr != 0) {
                    parsing_error(argv[0]);
                }            } else {
                parsing_error(argv[0]);
            }
            break;
        default:
            parsing_error(argv[0]);
            break;
        }
    }

    struct pam_conv conv = {misc_conv, &app_data}; /** La structure de gestion de conversation avec PAM*/
    pam_handle_t * pamh; /** Le handle pam */

    // Début du dialogue avec PAM.
    retval = pam_start("otpasswd", user, &conv, &pamh);
    if (retval != PAM_SUCCESS) {
        fprintf(stderr,
                "Erreur lors de l'initialisation de l'échange avec PAM");
        exit(EXIT_FAILURE);
    }
    // Demande de changement de secret.
    retval = pam_chauthtok(pamh, 0);
    if (retval != PAM_SUCCESS) {
        pam_end(pamh, 0);
        show_error("pam_chauthtok", retval);
        exit(EXIT_FAILURE);
    }
    // Libération des ressources.
    if (user != NULL) {
        free(user);
    }
    pam_end(pamh, 0);
    exit(EXIT_SUCCESS);
}
