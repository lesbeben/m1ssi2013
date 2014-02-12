#ifndef HOTP_H
#define HOTP_H
#include "secret.h"
#include "utils.h"


/** Génère un mot de passe à partir du secret en utilisant un compteur.
 * @param[in] key le secret utilisé pour générer un OTP
 * @param[in] counter le compteur permettant de générer un OTP.
 * @param[in] len la longueure de l'OTP a générer.
 * 
 * @pre secret != NULL.
 * @pre counter > 0
 * @pre len >= 6
 * @pre len <= 8
 * 
 * @return l'entier correspondant à l'OTP; -1 en cas d'échec
 */
int generateHOTP(secret key, long counter, int len);

#endif //TOTP_H
