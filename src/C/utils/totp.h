#ifndef TOTP_H
#define TOTP_H
#include "secret.h"

/** Génère un mot de passe à partir du secret en utilisant le temps UTC.
 * @param[in] key le secret utilisé pour générer un OTP
 * 
 * @return l'entier correspondant à l'OTP; -1 en cas d'échec
 */
int generateTOTP(secret key);

#endif //TOTP_H