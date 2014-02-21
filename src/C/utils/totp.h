#ifndef TOTP_H
#define TOTP_H
#include "secret.h"

/** \file
 * Fichier d'en-tête de fonctions pour générer des OTP basés sur la fonction
 * HMAC_SHA1 et le temps. C'est le fichier d'en-tête pour toute la libtotp.a.
 */

/** Génère un mot de passe à partir du secret en utilisant le temps UTC.
 * 
 * Cette fonction calcul un OTP à partir d'un secret et d'un quantum de
 * temps.<br/>
 * Le quantum représente la durée en secondes pendant la quelle l'OTP sera
 * valide.<br/>
 * L'algorithme de calcul se trouve dans la 
 * <a href="https://tools.ietf.org/html/rfc6238">RFC 6238</a>.
 * @param[in] key le secret utilisé pour générer un OTP
 * @param[in] len la taille de l'otp désiré.
 * @param[in] quantum la taille de la fenetre de temps en secondes.
 * @pre quantum > 0
 * @pre len > 0
 * @return l'entier correspondant à l'OTP; -1 en cas d'échec
 */
int generateTOTP(secret key, int quantum, int len);

#endif //TOTP_H
