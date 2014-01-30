#include <math.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include "totp.h"

int generateTOTP(secret key, int quantum, int len) {
    /*
    // Vérification de préconditions
    if (!((key != NULL) && (counter > 0) && (len > 0 && len <= 10))) {
        return -1;
    }
    //Allocation des buffers
    char * buffer = (char*) malloc(sizeof(char) * SHA_DIGEST_LENGTH);
    char * buffer2 = (char*) malloc(sizeof(char) * SHA_DIGEST_LENGTH);
    int32_t buffer3;
    time_t utcTime = time (NULL);
    buffer3 = (unsigned long) utcTime;
    //Etape 1 HMAC
    HMAC_SHA1(counter, key, buffer);
    //Etape 2 troncature
    truncate(buffer, buffer2);
    //Etape 3 conversion
    buffer3 = convert(buffer2);
    //Etape 4 Human writable buffer modulo 10^len
    free(buffer);
    free(buffer2);
    return (int)((int)buffer3 % (int)(pow(10,len)));
    */
    return -1;
}
