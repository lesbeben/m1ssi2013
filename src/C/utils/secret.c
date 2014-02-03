#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "secret.h"
#include "macros.h"

/*******************************************************************************
 *                                                                             *
 *                               UTILITAIRES                                   *
 *                                                                             *
 ******************************************************************************/
int getHexInt(char c) {
    if ('a' <= c && c <= 'f') {
        return c - 'a' + 10;
    } else if ('A' <= c && c <= 'F') {
        return c - 'A' + 10;
    } else if ('0' <= c && c <= '9') {
        return c - '0';
    }
    return -1;
}

int getHexCode(char * string) {
    int hexCode = (getHexInt(string[0]) << 4) | getHexInt(string[1]);
    return hexCode;
}

/*******************************************************************************
 *                                                                             *
 *                              FONCTIONS                                      *
 *                                                                             *
 ******************************************************************************/
secret createSecret(int length) {
    if (length < 0) { //Précondition invalide
        return NULL;
    }
    /** Le secret à retourner */
    secret res = (secret) malloc(sizeof(secret_struct));
    if (res == NULL) {
        return NULL;
    }

    //remplissage des variables
    res->length = length;
    res->buffer = (char *) malloc(sizeof(char) * length);

    if (res->buffer == NULL) {
        destroySecret(res);
        return NULL;
    }

    /** Descripteur de fichier sur /dev/urandom */
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        destroySecret(res);
        return NULL;
    }

    /** Code de retour d'appels système */
    int ret = read(fd,res->buffer, res->length);
    if (ret != res->length) {
        destroySecret(res);
        return NULL;
    }

    ret = close(fd);
    if (ret == -1) {
        destroySecret(res);
        return NULL;
    }

    return res;
}

secret textToSecret(char * buffer) {
    /** Le nombre de caractères du buffer */
    int length = strlen(buffer);
    /** Le secret à retourner initialisé avec le buffer */
    secret res = (secret) malloc(sizeof(secret_struct));
    if (res == NULL) {
        return NULL;
    }
    res->length = length;

    /** Création d'une copie du buffer dans le secret. */
    res->buffer = strndup(buffer, length);

    return res;
}

secret hexToSecret(char * buffer) {
    /** Le nombre de caractères dans le buffer */
    int length = strlen(buffer);
    /** Le secret à retourner initialisés avec les données du buffer.*/
    secret res = (secret) malloc(sizeof(secret_struct));
    if (res == NULL) {
        return NULL;
    }

    res->length = length / 2 + (IS_ODD(length)) ? 1 : 0;
    res->buffer = (char *) malloc(sizeof(char) * res->length);
    if (res->buffer == NULL) {
        destroySecret(res);
        return NULL;
    }

    /** Un pointeur temporaire sur un buffer contenant une chaîne héxadécimale
     * de longueur paire équivalent a celle contenue dans buffer
     */
    char * tmp;
    if (IS_ODD(length)) {
        length++;
        tmp = malloc(sizeof(char) * (length));
        snprintf(tmp, length + 1, "0%s", buffer);
    } else {
        tmp = buffer;
    }
    // On parcourt le buffer et on rempli le secret.
    for (int i = 0; i < res->length; i++) {
        res->buffer[i] = getHexCode(tmp + 2*i);
    }
    if (tmp != buffer) {
        free(tmp);
    }
    return res;
}

int destroySecret(secret key) {
    if (key == NULL) { //Précondition invalide.
        return -1;
    }
    if (key->buffer != NULL) {
        // Mise a 0 de la mémoire pour éviter l'exploitation d'attaques.
        memset(key->buffer, 0, key->length);
        free(key->buffer);
    }
    // Idem on met la mémoire a zéro pour éviter l'exploitation d'attaques.
    memset(key, 0, sizeof(secret_struct));
    free(key);
    return 0;
}

int getLength(secret key) {
    if (key == NULL) { // Précondition invalide.
        return -1;
    }
    return key->length;
}

char * getHexRepresentation(secret key, char * buffer, int length) {
    if( key == NULL ) {
        // Impossible de représenter d'un secret NULL
        return NULL;
    }
    else if(buffer == NULL) {
        // Impossible de donner la représentation sans avoir de buffer à remplir
        return NULL;
    } else if (length <= 0) {
        return NULL;
    } else if (!(length > (key->length * 2))) {
        return NULL;
    }
    for (int i = 0; i < key->length; i++) {
        // Remplissage du buffer avec les caractères hexadécimaux.
        if (snprintf(buffer + (2 * i), length, "%02x",
            (unsigned int) (unsigned char) (key->buffer[i])) < 0);
    }
    buffer[(key->length * 2)] = 0;
    printf("%s", buffer);
    return buffer;
}

char * getTextRepresentation(secret key, char* buffer, int length) {
    if( key == NULL ) {
        return NULL;
    }
    else if(buffer == NULL) {
        return NULL;
    } else if (length <= 0) {
        return NULL;
    } else if (!(length > key->length)) {
        return NULL;
    }
    /* Fin des tests de préconditions */
    // Remplissage du buffer avec key->buffer.
    strncpy(buffer, key->buffer, length);
    // Ajout du NULL-Byte.
    buffer[length] = 0;
    return buffer;
}
