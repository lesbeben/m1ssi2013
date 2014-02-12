#include <math.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include "hotp.h"

int generateHOTP(secret key, long counter, int len) {
    // Vérification de préconditions
    if ((key == NULL) || (counter < 0) || (len > 8) || (len < 6)) {
        return -1;
    }

    //Allocation des buffers
    char hmacResultBuffer[HMAC_SHA1_LENGTH];

    //Etape 1 HMAC
    if (HMAC_SHA1(counter, key, hmacResultBuffer) == NULL) {
        return -1;
    }

    // Étape 2 extraction de l'OTP depuis le résultat de hmac.
    int32_t fullLengthOtp = extractOTP(hmacResultBuffer);
    if (fullLengthOtp == -1) {
        return -1;
    }

    //Etape 3 Human writable buffer modulo 10^len
    return fullLengthOtp % (int) pow(10,len);
}
