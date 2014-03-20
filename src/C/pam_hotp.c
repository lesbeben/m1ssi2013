#include <stdlib.h>
#include <stdio.h>

#define _PAM_EXTERN_FUNCTIONS
#include <security/_pam_macros.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>

#include <sys/syslog.h>

#include "utils/hotp.h"
#include "utils/users.h"

int pam_sm_authenticate(pam_handle_t *pamh, int flags,
                        int argc, const char **argv) {

    const char * usrname;
    const char * otp;
    otpuser user;

    // Récupération du nom d'utilisateur dans name.
    if (pam_get_user(pamh, &usrname, NULL) != PAM_SUCCESS) {
        return PAM_USER_UNKNOWN;
    }

    // Obtention d'un OTP par PAM.
    if (pam_get_authtok(pamh, PAM_AUTHTOK, &otp, NULL) != PAM_SUCCESS) {
        return PAM_AUTH_ERR;
    }
    
    if (lockFile() == -1) {
        pam_syslog(pamh, LOG_ERR, "can't get lock");
        return PAM_AUTH_ERR;
    }

    // Récupération des données utilisateurs.
    if (getOTPUser(usrname, &user) == -1) {
        pam_syslog(pamh, LOG_ERR, "user unknown: %s", usrname);
        return PAM_USER_UNKNOWN;
    }

    // Vérification d'un mot de passe + resynch. 
    int otp_expected = 0;
    int otp_given = atoi(otp);
    for (int i = 0; i < 3; i++) {
        otp_expected = generateHOTP(user.passwd, user.params.count + i, 8);
        if (otp_expected == -1) {
            pam_syslog(pamh, LOG_ERR, "generateHOTP failed");
            unlockFile();
            return PAM_AUTH_ERR;
        }
        if (otp_expected == otp_given) {
            user.params.count += i;
            updateOTPUser(&user);
            if (unlockFile() == -1) {
                pam_syslog(pamh, LOG_ERR, "can't free lock on users");
            }
            return PAM_SUCCESS;
        } else {
            pam_syslog(pamh, LOG_ERR, "Auth denied expect %d", otp_expected);
        }
    }
    
    if (unlockFile() == -1) {
        pam_syslog(pamh, LOG_ERR, "can't free lock on users");
    }
    return PAM_AUTH_ERR;
}