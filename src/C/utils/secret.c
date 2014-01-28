#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "secret.h"

/*******************************************************************************
 *                                                                             *
 *                            UTILITAIRES                                      *
 *                                                                             *
 ******************************************************************************/

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

    //remplissage des variables
    res->length = length;
    res->buffer = (char *) malloc(sizeof(char) * length);

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
    secret res = createSecret(length / 2);
    if (res == NULL) {
        return NULL;
    }
    
    char octet = 0;
    /** Le code hexadécimal correspondant à deux caractères du buffer.*/
    int hexCode;
    
    /* On parcours le buffer deux caractères par caractères que l'on stockera
     * dans un octet (ici un char) puisque 2 caractères héxadécimaux tiennent
     * sur un octet.
     */
    for (int i = 0; i < length; i += 2) {
        hexCode = hexCharToInt(buffer[i]);
        if (hexCode == -1) { 
            // Buffer contient un caractère qui n'est pas hexadécimal.
            // Auquel cas on libère les ressources
            destroySecret(res);
            // Et on indique qu'il y a eu une erreur.
            return NULL;
        }
        //On masque pour ne prendre que les 4 premiers octets.
        octet |= (hexCode & 0x0F);
        //On décale les 4 octets vers la gauche (premier caractère hexa).
        octet <<= 4;
        hexCode = hexCharToInt(buffer[i + 1]);
        if (hexCode == -1) { 
            // Buffer contient un caractère qui n'est pas hexadécimal.
            // Même traitement.
            destroySecret(res);
            return NULL;
        }
        // Re masquage
        octet |= (hexCode & 0x0F);
        // On rentre le second caractère hexa.
        res->buffer[i / 2] = octet;
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
    return buffer;
}

char * getTextRepresentation(secret key, char* buffer, int length) {

}

