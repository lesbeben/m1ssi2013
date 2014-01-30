#include <math.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include "hotp.h"

int generateHOTP(secret key, unsigned long counter, int len) {
    // Vérification de préconditions
    if (!((key != NULL) && (counter > 0) && (len >= 6 && len <= 8))) {
        return -1;
    }
    //Allocation des buffers
    char * buffer = (char*) malloc(sizeof(char) * SHA_DIGEST_LENGTH);
    char * buffer2 = (char*) malloc(sizeof(char) * SHA_DIGEST_LENGTH);
    int32_t buffer3;
    //Etape 1 HMAC
    HMAC_SHA1(counter, key, buffer);
    //Etape 2 troncature
    truncate(buffer, buffer2);
    //Etape 3 conversion
    buffer3 = convert(buffer2);
    //Etape 4 Human writable buffer modulo 10^len
    free(buffer);
    free(buffer2);
    return (int)((int)buffer3 % ((int)(pow(10,len - 1))));
}
