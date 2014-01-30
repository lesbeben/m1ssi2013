#define _XOPEN_SOURCE 700
#include <string.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

#define IPAD 0x36
#define OPAD 0x5c


char* HMAC_SHA1(unsigned long count, secret key, char * buffer) {
    // Vérification de préconditions
    if (!((key != NULL) && (count != -1) && (buffer != NULL))) {
        return NULL;
    }

    // Copie du buffer pour les opérations XOR
    char * cpbuffer = (char*) malloc(sizeof(char) * (key->length));
    char * cp2buffer = (char*) malloc(sizeof(char) * (key->length));
    unsigned char hash[SHA_DIGEST_LENGTH];
    int bSize = key->length + 10;

    // XOR BIT A BIT K et IPAD
    for (int i = 0; i < key->length; i++) {
        cpbuffer[i] = key->buffer[i] ^ IPAD;
    }
    // Allocation d'un nouveau buffer pour concatenation
    char* tmp = (char *) malloc(sizeof(char) * bSize);

    // snprintf renvoie le nombre de caractères écrits dans le buffer.
    int ret = snprintf(tmp, bSize, "%s%ld", cpbuffer, count);

    // SHA1
    SHA1((unsigned char*)tmp, ret, hash);

    //Redefinition de la taille du buffer de passage
    bSize = key->length + SHA_DIGEST_LENGTH;

    //XOR BIT A BIT K et OPAD
    for (int i = 0; i < key->length; i++) {
        cp2buffer[i] = key->buffer[i] ^ OPAD;
    }

    //Allocation nouveau buffer pour concatenation
    char* tmp2= (char *) malloc(sizeof(char) * bSize);
    ret = snprintf(tmp2, bSize, "%s%ld", hash, count);
    SHA1((unsigned char*)tmp2, ret, hash);

    strncpy(buffer, (const char*)hash, SHA_DIGEST_LENGTH);

    //Libération ressources mémoire.
    free(tmp);
    free(tmp2);
    free(cpbuffer);
    free(cp2buffer);

    return buffer;
}

char* truncate(char * hash, char * buffer) {

    if (!((hash != NULL) && (buffer != NULL))) {
        return NULL;
    }
    char offset;
    offset = hash[19] & 0xF0;
    offset >>= 4;

    // int32_t == 4 octets, on prends donc les 4 octets en partant
    // de hash + offset.
    int32_t res = (int32_t) *(hash + offset);
    res &= 0x7FFFFFFF;
    buffer = memcpy(buffer, &res, 4 * sizeof(char));
    return buffer;
}

int32_t convert(char* buffer) {
    if(buffer == NULL) {
        return -1;
    }
    return (int32_t) (*buffer);
}
