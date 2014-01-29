#define _XOPEN_SOURCE 700
#include <string.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

#define IPAD 0x36
#define OPAD 0x5c


char* HMAC_SHA1(unsigned long count, secret key, char * buffer) {
    if ((key != NULL) && (count != -1) && (buffer != NULL)) {
        //Copie du buffer pour les opérations XOR
        char * cpbuffer =(char*) malloc(sizeof(char) * (key->length));
        char * cp2buffer =(char*) malloc(sizeof(char) * (key->length));
        unsigned char hash[SHA_DIGEST_LENGTH];
        int Bsize = key->length + 10;
        //XOR BIT A BIT K et IPAD
        for (int i = 0; i < key->length; i++) {
            cpbuffer[i] = key->buffer[i]^IPAD;
        }
        //Allocation nouveau buffer pour concatenation
        char* tmp= (char *) malloc(sizeof(char) * Bsize);
        int ret = snprintf(tmp, Bsize, "%s%ld", cpbuffer, count);

        //SHA1
        SHA1((unsigned char*)tmp, ret, hash);
        //Redefinition de la taille du buffer de passage
        Bsize = key->length + 20;
        //XOR BIT A BIT K et OPAD
        for (int i = 0; i < key->length; i++) {
            cp2buffer[i] = key->buffer[i]^OPAD;
        }
        //Allocation nouveau buffer pour concatenation
        char* tmp2= (char *) malloc(sizeof(char) * Bsize);
        ret = snprintf(tmp2, Bsize, "%s%ld", hash, count);
        SHA1((unsigned char*)tmp2, ret, hash);

        strncpy(buffer, (const char*)hash, 20);
        
        //Libération ressources mémoire.
        free(tmp);
        free(tmp2);
        free(cpbuffer);
        free(cp2buffer);
        
        return buffer;
    } else {
        return NULL;
    }
}

char* truncate(char * hash, char * buffer) {
    // TODO
    return NULL;
}

int32_t convert(char* buffer) {
    if(buffer == NULL) {
        return -1;
    }
    return (int) (*buffer);
}
