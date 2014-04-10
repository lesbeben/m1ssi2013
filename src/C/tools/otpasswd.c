#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <pam_appl.h>
#include <pam_misc.h>
#include <string.h>

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

int main(int argc, char * argv[]) {
    struct pam_conv conv = {misc_conv, NULL}; /** La structure de gestion de conversation avec PAM*/
    pam_handle_t * pamh; /** Le handle pam */
    char * user = NULL; /** Le nom de l'utilisateur */
    int retval; /** Stockage de valeur de retour. */

    // TODO: Parser argv
    if (argc == 2) {
        user = strdup(argv[1]);
    }

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
