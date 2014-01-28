#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "secret.h"

int hexCharToInt(char c) {
    if ('a' <= c && c <= 'f') {
        return c - 'a';
    } else if ('A' <= c && c <='F') {
        return c - 'A';
    } else if ('0' <= c && c <= '9') {
        return c - '0';
    }
    return -1;
}


secret createSecret(int length) {
    if (length < 0) { //Précondition invalide
        return NULL;
    }

    secret res = (secret) malloc(sizeof(secret_struct));

    //remplissage des variables
    res->length = length;
    res->buffer = (char *) malloc(sizeof(char) * length);

    return res;
}

secret hexToSecret(char * buffer) {
    int length = strlen(buffer);
    secret res = createSecret(length / 2);
    char octet = 0;
    int tmp;
    for (int i = 0; i < length; i += 2) {
        tmp = hexCharToInt(buffer[i]);
        octet |= tmp;
        octet <= 4;
        tmp = hexCharToInt(buffer[i + 1]);
        octet |= tmp;
        res->buffer[i / 2] = octet;
    }
    return res;
}

int destroySecret(secret key) {
    free(key->buffer);
    free(key);
    return 0;
}

int getLength(secret key) {
    return key->length;
}

char * getRepresentation(secret key, char * buffer, int length) {
    if( key == NULL ) {
        // Impossible de représenter d'un secret NULL
        return NULL;
    }
    else if( buffer == NULL ) {
        /* Impossible de donner la représentation sans avoir de buffer
         * à remplir.
         */
        return NULL;
    }
    // TODO
}
