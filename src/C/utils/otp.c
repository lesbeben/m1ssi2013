#include <math.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include "otp.h"

int generate_otp(secret key, long counter, int len) {
    // Vérification de préconditions
    if ((key == NULL) || (counter < 0) || (len > 8) || (len < 6)) {
        return OTP_ERR_PARAM;
    }

    // Buffer pour le résultat de HMAC_SHA1.
    char hmacResultBuffer[HMAC_SHA1_LENGTH];

    //Etape 1 HMAC
    if (HMAC_SHA1(counter, key, hmacResultBuffer) == NULL) {
        return OTP_ERR_HMAC;
    }

    // Étape 2 extraction de l'OTP depuis le résultat de HMAC_SHA1.
    int32_t fullLengthOtp = extractOTP(hmacResultBuffer);
    if (fullLengthOtp == -1) {
        return OTP_ERR_EXTRACT;
    }

    //Etape 3 Human writable buffer modulo 10^len
    return fullLengthOtp % (int) pow(10,len);
}
