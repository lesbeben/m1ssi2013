#ifndef HOTP_H
#define HOTP_H
#include "secret.h"

/** Génère un mot de passe à partir du secret en utilisant un compteur.
 * @param[in] key le secret utilisé pour générer un OTP
 * @pre secret != NULL.
 * @param[in] counter le compteur permettant de générer un OTP.
 * @pre counter > 0
 * 
 * @return l'entier correspondant à l'OTP; -1 en cas d'échec
 */
int generateHOTP(secret key, unsigned long counter);

#endif //TOTP_H
