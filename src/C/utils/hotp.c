#include <math.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include "hotp.h"

int generateHOTP(secret key, unsigned long counter, int len) {
    // Vérification de préconditions
    if (!((key != NULL) && (counter > 0) && (len >= 6 && len <= 8))) {
        return -1;
    }
    //Allocation des buffers
    char * hmacResultBuffer = (char*) malloc(sizeof(char) * SHA_DIGEST_LENGTH);
    //Etape 1 HMAC
    HMAC_SHA1(counter, key, hmacResultBuffer);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)  {
        printf("%02x", (unsigned char) (unsigned int) hmacResultBuffer[i]);
    }
    printf("\n");
    // Étape 2 extraction de l'OTP depuis le résultat de hmac.
    int32_t fullLengthOtp = extractOTP(hmacResultBuffer);
    printf("%d", fullLengthOtp);
    //Etape 3 Human writable buffer modulo 10^len
    return fullLengthOtp % (int) pow(10,len - 1);
}
