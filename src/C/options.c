#include <stdlib.h>

#include "options.h"

#define DELAY_TOTP "1"
#define DELAY_HOTP "3"

int fillflags(modopt* flag, int argc, const char** argv) {
    // Initialisation
    //char line[BUFFER_SIZE];
    char * endptr;
    const char *value;
    flag->use_auth_tok = 0;
    flag->null_ok = 0;
    set_opt(flag, DELAY_TOTP_AUTH, DELAY_TOTP);
    set_opt(flag, DELAY_HOTP_AUTH, DELAY_HOTP);
    
    for (int i =0; i < argc; i++) {
        if (!strcmp("use_auth_tok", argv[i])) {
            set_opt(flag, USE_AUTH_TOK, NULL);
            continue;
        }
        if (!strncmp("delay_totp", argv[i], 10)) {
            if (*(argv[i] + 10) == '=') {
                value = argv[i] + 11;
                if (*value != '\0') {
                    if (strtol (value, &endptr, 10) > 0) {
                        set_opt(flag, DELAY_TOTP_AUTH, value);
                    }
                }
            }
            continue;
        }
        if (!strncmp("delay_hotp", argv[i], 10)) {
            if (*(argv[i] + 10) == '=') {
                value = argv[i] + 11;
                if (*value != '\0') {
                    if (strtol (value, &endptr, 10) > 0) {
                        set_opt(flag, DELAY_HOTP_AUTH, value);
                    }
                }
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

int set_opt(modopt* flag, int field, const char* value) {
    char * endptr;
    switch(field) {
        case USE_AUTH_TOK:
            flag->use_auth_tok = 1;
            break;
        case DELAY_TOTP_AUTH:
            flag->delay_totp = (uint64_t)(strtol (value, &endptr, 10));
            if ((flag->delay_totp <= 0) || (*endptr != 0)) {
                return OPTIONS_ERROR;
            }
        case DELAY_HOTP_AUTH:
            flag->delay_hotp = (uint64_t)(strtol (value, &endptr, 10));
            if ((flag->delay_hotp <= 0) || (*endptr != 0)) {
                return OPTIONS_ERROR;
            }
        case NULL_OK:
            flag->null_ok = 1;
        default:
            return OPTIONS_ERROR;
    }
    return OPTIONS_ERROR;
}

int is_set(modopt* flag, int field) {
    switch(field) {
        case USE_AUTH_TOK:
            return flag->use_auth_tok;
        case NULL_OK:
            return flag->null_ok;
        default:
            return OPTIONS_SUCCESS;
    }
    return OPTIONS_SUCCESS;
}