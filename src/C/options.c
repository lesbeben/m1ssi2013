#include <stdlib.h>
#include <pam_ext.h>
#include <pam_appl.h>
#include <pam_misc.h>
#include <_pam_macros.h>
#include <pam_modules.h>
#include <syslog.h>

#include "options.h"

#define DEF_DELAY_TOTP 1
#define DEF_DELAY_HOTP 1

int parse_options(pam_handle_t * pamh, modopt* flag,
                  int argc, const char** argv) {

    // Pointeur sur une éventuelle valeur pour l'option.
    const char *value;

    // Mise en place des valeurs par défaut.
    flag->use_auth_tok = 0;
    flag->null_ok = 0;
    flag->delay.hotp = DEF_DELAY_HOTP;
    flag->delay.totp = DEF_DELAY_TOTP;

    // Boucle de lecture des options.
    for (int i =0; i < argc; i++) {
        if (!strcmp("use_auth_tok", argv[i])) {
            set_opt(flag, USE_AUTH_TOK, NULL);
            continue;
        }
        if (!strncmp("delay_totp", argv[i], 10)) {
            if (*(argv[i] + 10) == '=') {

                // Value point sur la valeur pour delay TOTP.
                value = argv[i] + 11;

                if (*value != '\0') { // La chaîne valeur n'est pas nulle
                    if (set_opt(flag, DELAY_TOTP_AUTH, value) == -1) {
                        // Echec lors de la lecture de la valeur.
                        pam_syslog(pamh, LOG_ERR,
                                   "invalid value for delay_totp: %s, using default value :%d.",
                                   value, DEF_DELAY_TOTP);
                        flag->delay.totp = DEF_DELAY_TOTP;
                    }
                } else { // La chaîne valeur est nulle
                    pam_syslog(pamh, LOG_ERR,
                               "no value specified for delay_totp, using default value: %d.",
                               DEF_DELAY_TOTP);
                }
            } else { // Pas de valeur spécifiée
                pam_syslog(pamh, LOG_ERR,
                           "no value specified for delay_totp, using default value: %d.",
                           DEF_DELAY_TOTP);
            }
            continue;
        }
        if (!strncmp("delay_hotp", argv[i], 10)) {
            if (*(argv[i] + 10) == '=') {
                // Pointeur sur la valeur du délai pour hotp.
                value = argv[i] + 11;
                if (*value != '\0') { // La valeur n'est pas nulle.
                    if (set_opt(flag, DELAY_HOTP_AUTH, value) == -1) {
                        // Erreur lors de la lecture de la valeur de l'option.
                        pam_syslog(pamh, LOG_ERR,
                                   "invalid value for delay_hotp: %s, using default value: %d.",
                                   value, DEF_DELAY_HOTP);
                        flag->delay.hotp =  DEF_DELAY_HOTP;
                    }
                } else { // Pas de valeur fournie.
                    pam_syslog(pamh, LOG_ERR,
                               "no value specified for delay_hotp, using default value: %d.",
                               DEF_DELAY_HOTP);
                }
            } else { // Pas de valeur fournie.
                pam_syslog(pamh, LOG_ERR,
                           "no value specified for delay_hotp, using default value: %d.",
                           DEF_DELAY_HOTP);
            }
            continue;
        }
        if (!strncmp("null_ok", argv[i], 7)) {
            set_opt(flag, NULL_OK, NULL);
            continue;
        }
        pam_syslog(pamh, LOG_ERR, "unknown option: %s", argv[i]);
    }
    return OPTIONS_SUCCESS;
}

int set_opt(modopt* flag, int field, const char * value) {
    // Pointeur pour vérifier si strtol fonctionne.
    char * endptr;
    switch(field) {
    case USE_AUTH_TOK:
        flag->use_auth_tok = 1;
        break;
    case DELAY_TOTP_AUTH:
        flag->delay.totp = (uint64_t)(strtol (value, &endptr, 10));
        if ((flag->delay.totp <= 0) || (*endptr != 0)) {
            return OPTIONS_ERROR;
        }
        break;
    case DELAY_HOTP_AUTH:
        flag->delay.hotp = (uint64_t)(strtol (value, &endptr, 10));
        if ((flag->delay.hotp <= 0) || (*endptr != 0)) {
            return OPTIONS_ERROR;
        }
        break;
    case NULL_OK:
        flag->null_ok = 1;
        break;
    default:
        return OPTIONS_ERROR;
    }
    return OPTIONS_SUCCESS;
}

int get_opt(modopt* flag, int field) {
    switch(field) {
    case USE_AUTH_TOK:
        return flag->use_auth_tok;
    case NULL_OK:
        return flag->null_ok;
    case DELAY_HOTP_AUTH:
        return flag->delay.hotp;
    case DELAY_TOTP_AUTH:
        return flag->delay.totp;
    default:
        return OPTIONS_SUCCESS;
    }
    return OPTIONS_SUCCESS;
}
