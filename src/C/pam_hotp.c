#include <stdlib.h>
#include <stdio.h>

#define _PAM_EXTERN_FUNCTIONS
#include <_pam_macros.h>
#include <pam_modules.h>
#include <pam_ext.h>

#include <sys/syslog.h>

#include "utils/otp.h"
#include "utils/users.h"

/** TODO:
 *  - Gérer la mise à jour de secret/création de compte: voir ligne 111.
 */

/** Vérifie que qu'un utilisateur entre le bon OTP.
 *
 * Seule fonction qui devrait vraiment différer entre pam_hotp et pam_totp.
 */
int _check_otp(pam_handle_t * pamh, const char * username, const char * otp) {
    otpuser user;
    if (lockFile() == -1) {
        pam_syslog(pamh, LOG_ERR, "can't get lock");
        return PAM_AUTH_ERR;
    }

    // Récupération des données utilisateurs.
    if (getOTPUser(username, &user) == -1) {
        pam_syslog(pamh, LOG_ERR, "bad username %s", username);
        return PAM_USER_UNKNOWN;
    }

    // Vérification d'un mot de passe + resynch.
    int otp_expected = 0;
    int otp_given = atoi(otp);
    for (int i = 0; i < 3; i++) {
        otp_expected = generate_otp(user.passwd, user.params.count + i, user.otp_len);
        if (otp_expected == -1) {
            pam_syslog(pamh, LOG_ERR, "generate_otp failed");
            unlockFile();
            return PAM_AUTH_ERR;
        }
        if (otp_expected == otp_given) {
            user.params.count += i + 1;
            updateOTPUser(&user);
            if (unlockFile() == -1) {
                pam_syslog(pamh, LOG_ERR, "can't free lock on users");
            }
            pam_syslog(pamh, LOG_NOTICE, "%s logged in", username);
            return PAM_SUCCESS;
        }
    }
    pam_syslog(pamh, LOG_ERR,"%s failed to log", username);

    if (unlockFile() == -1) {
        pam_syslog(pamh, LOG_ERR, "can't free lock on users");
    }
    return PAM_AUTH_ERR;
}

int pam_sm_authenticate(pam_handle_t *pamh, int flags,
                        int argc, const char **argv) {

    const char * usrname;
    char * otp;
    int retval;
    // Récupération du nom d'utilisateur dans name.
    if ((retval = pam_get_user(pamh, &usrname, NULL)) != PAM_SUCCESS) {
        return retval;
    }
    if (usrname == NULL) {
        return PAM_USER_UNKNOWN;
    }

    // Obtention d'un OTP par PAM.
    if ((retval = pam_prompt(pamh, PAM_PROMPT_ECHO_ON,
                             &otp, "Mot de passe jetable: "))
        != PAM_SUCCESS) {
        return retval;
    }


    return _check_otp(pamh, usrname, otp);
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

PAM_EXTERN int pam_sm_chauthtok (pam_handle_t *pamh, int flags,
                                 int argc, const char **argv) {
    // Obtenir le nom d'utilisateur.
    const char * username;
    int retval = pam_get_user(pamh,&username, NULL);
    if (retval != PAM_SUCCESS) {
        return retval;
    }
    // Tester phase.
    if (flags & PAM_PRELIM_CHECK) {
        // - PRELIM, vérification avant de mettre à jour les informations.
        //   - Test si l'utilisateur a un compte actif.
        if (!userExists(username)) {
            // Aucun compte existant, pré requis pour mettre à jour le secret
            // validé.
            return PAM_SUCCESS;
        }
        // L'utilisateur à un compte il faut vérifier qu'il en est
        // le propriétaire.
        return PAM_TRY_AGAIN;
    }
    //  - PRELIM OK
    //    - Prompter pour nouveau secret.

    return PAM_PERM_DENIED;
}
