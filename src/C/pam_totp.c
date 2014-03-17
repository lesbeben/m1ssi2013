#include <stdlib.h>
#include <stdio.h>

#define _PAM_EXTERN_FUNCTIONS
#include <security/_pam_macros.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>

#include "utils/hotp.h"
#include "utils/users.h"

int pam_sm_authenticate(pam_handle_t *pamh, int flags,
                        int argc, const char **argv) {

    const char * name;
    otpuser user;

    // Récupération du nom d'utilisateur dans name.
    pam_get_user(pamh, &name, NULL);
    // Récupération des données utilisateurs.
    if (getOTPUser(name, &user) == -1) {
        return PAM_USER_UNKNOWN;
    }
    // Obtention d'un OTP.
    // Vérification d'un mot de passe.
    //  - Éventuelle tentative de resynchronisation.
    return PAM_AUTH_ERR;
}