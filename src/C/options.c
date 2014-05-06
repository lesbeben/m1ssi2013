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
    // Initialisation
    const char *value;
    flag->use_auth_tok = 0;
    flag->null_ok = 0;
    flag->delay.hotp = DEF_DELAY_HOTP;
    flag->delay.totp = DEF_DELAY_TOTP;
    
    for (int i =0; i < argc; i++) {
        if (!strcmp("use_auth_tok", argv[i])) {
            set_opt(flag, USE_AUTH_TOK, NULL);
            continue;
        }
        if (!strncmp("delay_totp", argv[i], 10)) {
            if (*(argv[i] + 10) == '=') {
                value = argv[i] + 11;
                if (*value != '\0') {
                    if (set_opt(flag, DELAY_TOTP_AUTH, value) == -1) {
                        pam_syslog(pamh, LOG_ERR,
                "invalid value for delay_totp: %s, using default value :%d.", 
                        value, DEF_DELAY_TOTP);
                        flag->delay.totp = DEF_DELAY_TOTP;
                    }
                } else {
                    pam_syslog(pamh, LOG_ERR,
                "no value specified for delay_totp, using default value: %d.",
                    DEF_DELAY_TOTP);
                }
            } else {
                pam_syslog(pamh, LOG_ERR,
                "no value specified for delay_totp, using default value: %d.",
                DEF_DELAY_TOTP);
            }
            continue;
        }
        if (!strncmp("delay_hotp", argv[i], 10)) {
            if (*(argv[i] + 10) == '=') {
                value = argv[i] + 11;
                if (*value != '\0') {
                    if (set_opt(flag, DELAY_HOTP_AUTH, value) == -1) {
                        pam_syslog(pamh, LOG_ERR,
                "invalid value for delay_hotp: %s, using default value: %d.",
                        value, DEF_DELAY_HOTP);
                        flag->delay.hotp =  DEF_DELAY_HOTP;
                    }
                } else {
                    pam_syslog(pamh, LOG_ERR,
                "no value specified for delay_hotp, using default value: %d.",
                DEF_DELAY_HOTP);
                } 
            }else {
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
    }
    return OPTIONS_SUCCESS;
}

int set_opt(modopt* flag, int field, const char * value) {
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
        case DELAY_HOTP_AUTH:
            flag->delay.hotp = (uint64_t)(strtol (value, &endptr, 10));
            if ((flag->delay.hotp <= 0) || (*endptr != 0)) {
                return OPTIONS_ERROR;
            }
        case NULL_OK:
            flag->null_ok = 1;
        default:
            return OPTIONS_ERROR;
    }
    return OPTIONS_ERROR;
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