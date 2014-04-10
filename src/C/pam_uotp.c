#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define _PAM_EXTERN_FUNCTIONS
#include <_pam_macros.h>
#include <pam_modules.h>
#include <pam_ext.h>

#include <sys/syslog.h>
#include <string.h>

#include "utils/otp.h"
#include "utils/users.h"
#include "options.h"


/** TODO:
 *  - Gérer la mise à jour de secret/création de compte: 
 *    voir ligne pam_sm_chauthtok.
 */

/** Vérifie que l'otp est valide pour l'utilisateur passé en paramètre.
 * 
 */
int _check_totp(pam_handle_t * pamh, otpuser * user, const char * otp) {
    // Vérification d'un mot de passe.
    int otp_expected = 0;
    int otp_given = atoi(otp);
    long lastAuth = user->params.totp.tps;
    int hasFound = 0;
    //Check if banned
    if (user->isBanned > 2) {
        pam_syslog(pamh, LOG_ERR, "User banned");
        return PAM_AUTH_ERR;
    }
    for (int i = -2; i <= 3 && !hasFound; i++) {
        int delta = (user->params.totp.delay + i) * user->params.totp.quantum;
        long counter = (time(NULL) + delta) / user->params.totp.quantum;
        if (lastAuth < counter) {
            otp_expected = generate_otp(user->passwd, counter, user->otp_len);
            if (otp_expected < OTP_SUCCESS) {
                pam_syslog(pamh, LOG_ERR, "TOTP generation failed errcode=%d",
                           otp_expected);
                unlockFile();
                return PAM_AUTH_ERR;
            }

            if (otp_expected == otp_given) {
                hasFound = 1;
                user->isBanned = 0;
                user->params.totp.tps = counter;
                user->params.totp.delay += i;
                updateOTPUser(user);
                pam_syslog(pamh, LOG_NOTICE, "%s logged in", user->username);
                return PAM_SUCCESS;
            }
        }
    }
    
    user->isBanned += 1;
    updateOTPUser(user);

    pam_syslog(pamh, LOG_NOTICE, "%s failed to log in", user->username);
    if (!hasFound) {
        pam_syslog(pamh, LOG_ERR, "can't synchronize");
    }
    return PAM_AUTH_ERR;
}

/** Vérifie que qu'un utilisateur entre le bon OTP.
 */
int _check_hotp(pam_handle_t * pamh, otpuser * user, const char * otp) {
    // Vérification d'un mot de passe + resynch.
    int otp_expected = 0;
    int otp_given = atoi(otp);
    //Check if banned
    if (user->isBanned > 2) {
        pam_syslog(pamh, LOG_ERR, "User banned");
        return PAM_AUTH_ERR;
    }
    for (int i = 0; i < 3; i++) {
        otp_expected = generate_otp(user->passwd, user->params.hotp.count + i,
                                    user->otp_len);
        if (otp_expected < OTP_SUCCESS) {
            pam_syslog(pamh, LOG_ERR, "generate_otp failed");
            unlockFile();
            return PAM_AUTH_ERR;
        }
        if (otp_expected == otp_given) {
            user->isBanned = 0;
            user->params.hotp.count += i + 1;
            updateOTPUser(user);
            if (unlockFile() != USR_SUCCESS) {
                pam_syslog(pamh, LOG_ERR, "can't free lock on users");
            }
            pam_syslog(pamh, LOG_NOTICE, "%s logged in", user->username);
            return PAM_SUCCESS;
        }
    }
    
    user->isBanned += 1;
    updateOTPUser(user);
    
    pam_syslog(pamh, LOG_ERR,"%s failed to log in", user->username);

    if (unlockFile() != USR_SUCCESS) {
        pam_syslog(pamh, LOG_ERR, "can't free lock on users");
    }
    return PAM_AUTH_ERR;
}
/** Vérifie que qu'un utilisateur entre le bon OTP.
 *
 * Seule fonction qui devrait vraiment différer entre pam_hotp et pam_totp.
 */
int _check_otp(pam_handle_t * pamh, const char * username, const char * otp) {
    otpuser user;
    if (lockFile() != USR_SUCCESS) {
        pam_syslog(pamh, LOG_ERR, "can't get lock");
        return PAM_AUTH_ERR;
    }

    // Récupération des données utilisateurs.
    if (getOTPUser(username, &user) != USR_SUCCESS) {
        pam_syslog(pamh, LOG_ERR, "bad username %s", username);
        return PAM_USER_UNKNOWN;
    }
    int retval;
    switch(user.method) {
    case HOTP_METHOD:
        retval = _check_hotp(pamh, &user, otp);
        break;
    case TOTP_METHOD:
        retval = _check_totp(pamh, &user, otp);
        break;
    default:
        pam_syslog(pamh, LOG_ERR, "Unknown otp method");
        retval = PAM_AUTH_ERR;
        break;
    }

    if (unlockFile() == -1) {
        pam_syslog(pamh, LOG_ERR, "can't free lock");
    }
    return retval;
}


int pam_sm_authenticate(pam_handle_t *pamh, int flags,
                        int argc, const char **argv) {

    const char * usrname;
    const char *otp2;
    char * otp;
    int retval;

    modopt  modstr;

    // Récupération du nom d'utilisateur dans name.
    if ((retval = pam_get_user(pamh, &usrname, NULL)) != PAM_SUCCESS) {
        return retval;
    }
    if (usrname == NULL) {
        return PAM_USER_UNKNOWN;
    }

    // Obtention d'un OTP par PAM.
    if (fillflags(&modstr, argc, argv) == -1) {
        pam_syslog(pamh, LOG_ERR, "No options");
    }
    if (is_set(&modstr, USE_AUTH_TOK)) {
        if ((retval = pam_get_authtok(pamh, PAM_AUTHTOK,
                                      &otp2, "Mot de passe jetable: "))
                != PAM_SUCCESS) {
            return retval;
        }
        if (otp2 == NULL) {
            return PAM_AUTH_ERR;
        }

        /* Coutournement de la clause const on passe par un autre buffer
         * qui lui est modifiable.
         */
        int len = strlen(otp2);
        otp = malloc(sizeof(char) * (len + 1));
        if (otp == NULL) {
            return PAM_AUTH_ERR;
        }
        memcpy(otp, otp2, len * sizeof(char));
        otp[len] = 0;

        /* Un otp est par définition temporaire on le supprime donc du cache
         * de PAM pour qu'un autre module ne plante pas en récupérant un OTP.
         */
        pam_set_item(pamh, PAM_AUTHTOK, NULL);
    } else {
        if ((retval = pam_prompt(pamh, PAM_PROMPT_ECHO_ON,
                                 &otp, "Mot de passe jetable: "))
                != PAM_SUCCESS) {
            return retval;
        }
    }

    retval = _check_otp(pamh, usrname, otp);
    if (is_set(&modstr, USE_AUTH_TOK)) {
        free(otp);
    }
    return retval;
}

/** Identique à pam_unix */
int pam_sm_setcred (pam_handle_t *pamh, int flags,
                    int argc, const char **argv) {
    int retval;
    const void *pretval = NULL;

    D(("called."));

    retval = PAM_SUCCESS;

    D(("recovering return code from auth call"));
    /* We will only find something here if UNIX_LIKE_AUTH is set --
     *      don't worry about an explicit check of argv. */
    if (pam_get_data(pamh, "unix_setcred_return", &pretval) == PAM_SUCCESS
            && pretval) {
        retval = *(const int *)pretval;
        pam_set_data(pamh, "unix_setcred_return", NULL, NULL);
        D(("recovered data indicates that old retval was %d", retval));
    }

    return retval;
}

int pam_sm_chauthtok (pam_handle_t *pamh, int flags,
                      int argc, const char **argv) {
    int retval;
    char *otp;
    
    // Obtenir le nom d'utilisateur.
    const char * username;
    if ((retval = pam_get_user (pamh, &username, NULL)) != PAM_SUCCESS) {
        return retval;
    }

    if (username == NULL) {
        pam_syslog (pamh, LOG_ERR, "Unknown user");
        return PAM_USER_UNKNOWN;
    }

    // 1er appel de la fonction avec le flag PAM_PRELIM_CHECK.
    // Test si l'utilisateur a un compte actif.
    if (flags & PAM_PRELIM_CHECK) {
        const char *cstotp;

        // Tout d'abord, qui éxecute le processus courant ?
        if (geteuid () == 0) {
            // Si l'utilisateur est root, alors pas de conditions 
            // supplémentaires, l'utilisateur à l'autorité
            return PAM_SUCCESS;
        }

        switch (retval = userExists (username)) {
            case 0 : 
                // Aucun compte existant, pré requis pour mettre à jour le 
                // secret validé.
                return PAM_SUCCESS;
            case 1 :
                // Compte existant, il faudra vérifier s'il en est le 
                // propriétaire
                break;
            default :
                // Une erreur s'est produite
                pam_syslog (pamh, LOG_ERR, 
                    "user doesn't exists in otpasswd file");
                return PAM_AUTHTOK_ERR;
        }

        // Vérifier l'autheticité du propriétaire par une demande 
        // d'authentification (pas d'appel à pam_sm_authenticate pour ne pas 
        // récupérer une deuxième fois le user)
        if ((retval = pam_get_authtok (pamh, PAM_AUTHTOK, &cstotp, 
                "Mot de passe jetable: ")) != PAM_SUCCESS) {
            return retval;
        }

        if (cstotp == NULL) {
            return PAM_AUTH_ERR;
        }

        int otpLen = strlen (cstotp);
        otp = malloc (sizeof (char) * (otpLen + 1));
        if (otp == NULL) {
            return PAM_AUTH_ERR;
        }
        memcpy (otp, cstotp, otpLen * sizeof (char));
        otp[otpLen] = 0;
        // Suppression de l'otp du cache
        pam_set_item (pamh, PAM_AUTHTOK, NULL);

        retval = _check_otp (pamh, username, otp);
        free(otp);
        if (retval == PAM_SUCCESS) {
            return PAM_SUCCESS;
        }

        pam_syslog (pamh, LOG_ERR, "user authentication failed");
        return PAM_TRY_AGAIN;
    }

    //  2eme appel de la fonction avec le flag PAM_UPDATE_AUTHTOK.
    if (flags & PAM_UPDATE_AUTHTOK) {
        otpuser user;
        int secretLen;
        char * retstr;

        /* On recréé l'utilisateur de zéro s'il existe déjà */

        // Nom
        strcpy (user.username, username);

        // Demande de la méthode d'authentification
        if ((retval = pam_prompt (pamh, PAM_PROMPT_ECHO_ON, &retstr, 
            "Méthode d'authentification (hotp/totp) : ")) != PAM_SUCCESS) {
            return retval;
        }

        if (strncmp (retstr, "hotp", 5) == 0) {
            user.method = 'h';

            // Demande du quantum
            char * quantum;
            if ((retval = pam_prompt (pamh, PAM_PROMPT_ECHO_ON, &quantum, 
                "Quantum : ")) != PAM_SUCCESS) {
                return retval;
            }

            user.params.totp.tps = 0;
            user.params.totp.delay = atoi (quantum);

            free (quantum);
        } else if (strncmp (retstr, "totp", 5) == 0) {
            user.method = 't';
            user.params.hotp.count = 0;
        } else {
            return PAM_AUTHTOK_ERR;
        }

        // Le secret
        secretLen = getLength (user.passwd);
        user.passwd = createRandomSecret (secretLen);

        // Demande de la longueur des mots de passe générés
        if ((retval = pam_prompt (pamh, PAM_PROMPT_ECHO_ON, &retstr, 
            "Taille des mots de passe : ")) != PAM_SUCCESS) {
            return retval;
        }
        user.otp_len = (char) atoi (retstr);
        free (retstr);

        // État de l'utilisateur
        user.isBanned = 0;


        /* Mise à jour du fichier otpasswd */
        if (updateOTPUser (&user) != PAM_SUCCESS) {
            pam_syslog (pamh, LOG_ERR, "user update failed");
            return PAM_PERM_DENIED;
        }
        
        // Prompt du nouveau secret
        otp = malloc (secretLen * sizeof (char));
        getHexRepresentation (user.passwd, otp, secretLen);
        pam_info (pamh, "Le nouveau secret est : %s", otp);
        free (otp);

        return PAM_SUCCESS;
    }

    return PAM_PERM_DENIED;
}
