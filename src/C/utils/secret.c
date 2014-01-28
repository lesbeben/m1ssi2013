#include "secret.h"
#include <string.h>

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

int destroySecret(secret key) {
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
