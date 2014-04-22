#include <stdlib.h>

#include "options.h"

#define SEPARATOR "="
#define BUFFER_SIZE 32

int fillflags(modopt* flag, int argc, const char** argv) {
    // Initialisation
    //char line[BUFFER_SIZE];
    char * endptr;
    const char *tmp;
    
    for (int i =0; i < argc; i++) {
        if (!strcmp("use_auth_tok", argv[i])) {
            set_opt(flag, USE_AUTH_TOK, NULL);
            continue;
        }
        if (!strncmp("delay_totp", argv[i], 10)) {
            tmp = argv[i] + 11;
            if (!strcmp("=", argv[i] + 10)) {
                if (*tmp != '\0') {
                    if (strtol (tmp, &endptr, 10) > 0) {
                        set_opt(flag, DELAY_TOTP_AUTH, tmp);
                    }
                }
            }
            continue;
        }
        if (!strncmp("delay_hotp", argv[i], 10)) {
            tmp = argv[i] + 11;
            if (!strcmp("=", argv[i] + 10)) {
                if (*tmp != '\0') {
                    if (strtol (tmp, &endptr, 10) > 0) {
                        set_opt(flag, DELAY_HOTP_AUTH, tmp);
                    }
                }
            }
            continue;
        }
    }
    return 0;
}

int set_opt(modopt* flag, int field, const char* value) {
    char * endptr;
    switch(field) {
        case USE_AUTH_TOK:
            flag->use_auth_tok = 1;
            break;
        case DELAY_TOTP_AUTH:
            ;
            flag->delay_totp = (uint64_t)(strtol (value, &endptr, 10));
        case DELAY_HOTP_AUTH:
            flag->delay_hotp = (uint64_t)(strtol (value, &endptr, 10));
        default:
            return -1;
    }
    return -1;
}

int is_set(modopt* flag, int field) {
    switch(field) {
        case USE_AUTH_TOK:
            return flag->use_auth_tok;
        case DELAY_TOTP_AUTH:
            if (flag->delay_totp > 0) {
                return 1;
            } else {
                 return 0;
            }
        case DELAY_HOTP_AUTH:
            if (flag->delay_hotp > 0) {
                return 1;
            } else {
                return 0;
            }
        default:
            return 0;
    }
    return 0;
}