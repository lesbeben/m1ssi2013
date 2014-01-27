#include "secret.h"
#include <string.h>

secret createSecret(int length) {
    //creation du pointeur sur le secret
    secret res;
    
    //remplissage des variables
    res->length = length;
    res->buffer = malloc(length);
    
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
        //"key NULL"
		return NULL;
    }
    else if( buffer == NULL ) {
        //"buffer NULL"
		return NULL;
    }
    
    
    
    // TODO
}
