#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define _PAM_EXTERN_FUNCTIONS
#include <security/_pam_macros.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>

#include <sys/syslog.h>

#include "utils/totp.h"
#include "utils/users.h"

#define QUANTUM 30
#define OTP_LENGTH 8

int pam_sm_authenticate(pam_handle_t *pamh, int flags,
                        int argc, const char **argv) {

    const char * name;
    const char * otp;
    otpuser user;

    // Récupération du nom d'utilisateur dans name.
    pam_get_user(pamh, &name, NULL);

    // Obtention d'un OTP.
    if (pam_get_authtok(pamh,PAM_AUTHTOK,&otp,NULL) != PAM_SUCCESS) {
        return PAM_AUTH_ERR;
    }
    
    if (lockFile() == -1) {
        pam_syslog(pamh, LOG_ERR, "can't get lock");
        return PAM_AUTH_ERR;
    }
    
    // Récupération des données utilisateurs.
    if (getOTPUser(name, &user) == -1) {
        pam_syslog(pamh, LOG_ERR, "Error user :%s", name);
        return PAM_USER_UNKNOWN;
    }
    
    // Vérification d'un mot de passe.
    int otp_expected = 0;
    int otp_given = atoi(otp);
    long lastAuth = user.params.tps / QUANTUM;
    int hasFound = 0;
    for (int i = -2; i <= 3 && !hasFound; i++) {
        int delta = i* QUANTUM;
        long counter = time(NULL) + delta;
        if (lastAuth <= counter) {
            otp_expected = generateTOTP(user.passwd, QUANTUM, counter, OTP_LENGTH);
            
            if (otp_expected == -1) {
                pam_syslog(pamh, LOG_ERR, "generateTOTP failed");
                unlockFile();
                return PAM_AUTH_ERR;
            }
            
            if (otp_expected == otp_given) {
                hasFound = 1;
                user.params.tps = counter;
                updateOTPUser(&user);
                if (unlockFile() == -1) {
                    pam_syslog(pamh, LOG_ERR, "can't free lock");
                }
                return PAM_SUCCESS;
            }
        }
    }
    if (!hasFound) {
        pam_syslog(pamh, LOG_ERR, "can't synchronize");
    }
    
    if (unlockFile() == -1) {
        pam_syslog(pamh, LOG_ERR, "can't free lock");
    }
    return PAM_AUTH_ERR;
}