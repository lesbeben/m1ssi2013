#define _XOPEN_SOURCE 700
#define _BSD_SOURCE
#include <string.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <endian.h>

#define IPAD 0x36
#define OPAD 0x5c
#define HMAC_KEY_LENGTH 64


char* HMAC_SHA1(uint64_t count, secret key, char* buffer) {
    // Vérification de préconditions
    if ((key == NULL) || (count < 0) || (buffer == NULL)) {
        return NULL;
    }

    //Buffer de gestion du secret.
    unsigned char * temporarySecret;
    if (key->length == HMAC_KEY_LENGTH) {
        temporarySecret = (unsigned char *)key->buffer;
    } else {
        temporarySecret = malloc(HMAC_KEY_LENGTH * sizeof(char));
        memset(temporarySecret, 0, HMAC_KEY_LENGTH);
        if (key->length > 64) {
            SHA1((unsigned char *)key->buffer, key->length, temporarySecret);
        } else {
            memcpy(temporarySecret, key->buffer, key->length);
        }
    }

    // Copie du buffer pour les opérations XOR
    unsigned char xorSecretIpad[HMAC_KEY_LENGTH + sizeof(uint64_t)];
    unsigned char xorSecretOpad[HMAC_KEY_LENGTH + SHA_DIGEST_LENGTH];

    //On passe le compteur en représentation réseau.
    uint64_t tmpCounter = htobe64(count);

    // XOR BIT A BIT K et IPAD, K et OPAD.
    for (int i = 0; i < HMAC_KEY_LENGTH; i++) {
        xorSecretIpad[i] = temporarySecret[i] ^ IPAD;
        xorSecretOpad[i] = temporarySecret[i] ^ OPAD;
    }
    //On concatène count à notre xor bit à bit entre k et ipad
    memcpy(xorSecretIpad + HMAC_KEY_LENGTH, &tmpCounter, sizeof(uint64_t));
    // On hache le tout et on le mets à la suite du xor bit à bit de
    // la clé et de opad.
    SHA1(xorSecretIpad, HMAC_KEY_LENGTH + sizeof(uint64_t),
         xorSecretOpad + HMAC_KEY_LENGTH);
    // On hache une dernière fois et on rempli buffer avec ce haché.
    SHA1(xorSecretOpad, HMAC_KEY_LENGTH + SHA_DIGEST_LENGTH, (unsigned char*) buffer);
    return buffer;
}

int32_t extractOTP(char* hash) {
    if (hash == NULL) { // Préconditions invalides.
        return -1;
    }
    // On va d'abord déterminer l'offset pour cela on va manipuler le dernier
    // octet du haché, le masque 0xF0 sert a annuler les bits de poid faible.
    // Ensuite on décale les bits de poids fort vers les bits de poid faible
    // pour obtenir un entier sur 4 bits.
    char offset = (hash[19] & 0x0F);

    // On va ensuite prendre les 4 premiers octets à partir de offset dans le
    // haché. On va pour cela utiliser le type int32_t qui compte 4 octets et
    // initialisé ses octets avec la valeur contenu dans le haché a partir de
    // l'offset.
    int32_t fullLengthOTP = 0;
    for (int i = 0; i < 4; i++) {
        fullLengthOTP <<= 8;
        fullLengthOTP = fullLengthOTP | (0xFF & ((char) hash[offset + i]));
    }
    // Ce masque permet de ne pas tenir compte du signe de l'entier.
    return fullLengthOTP  & 0x7FFFFFFF;
}
