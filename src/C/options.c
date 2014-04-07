#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "options.h"


int fillflags(modopt* flag, int argc, const char** argv) {
    for (int i =0; i < argc; i++) {
        if (!strcmp("use_auth_tok", argv[i])) {
            set_opt(flag, USE_AUTH_TOK);
            continue;
        }
    }
    return 0;
}

int set_opt(modopt* flag, int field) {
    switch(field) {
        case USE_AUTH_TOK:
            flag->use_auth_tok = 1;
            break;
        default:
            return -1;
    }
    return -1;
}

int is_set(modopt* flag, int field) {
    switch(field) {
        case USE_AUTH_TOK:
            return flag->use_auth_tok;
        default:
            return 0;
    }
    return 0;
}