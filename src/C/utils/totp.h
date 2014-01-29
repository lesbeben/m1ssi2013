#ifndef TOTP_H
#define TOTP_H
#include "secret.h"

/** Génère un mot de passe à partir du secret en utilisant le temps UTC.
 * @param[in] key le secret utilisé pour générer un OTP
 * @param[in] len la taille de l'otp désiré.
 * @param[in] quantum la taille de la fenetre de temps
 * @pre quantum > 0
 * @pre len > 0
 * @return l'entier correspondant à l'OTP; -1 en cas d'échec
 */
int generateTOTP(secret key, int quantum, int len);

#endif //TOTP_H
