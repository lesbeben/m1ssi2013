#ifndef HOTP_H
#define HOTP_H
#include "secret.h"
#include "utils.h"

/** \file
 * Fichier d'en-tête de fonctions pour générer des OTP basés sur la fonction
 * HMAC_SHA1. C'est le fichier d'en-tête pour toute la libhotp.a.
 */

/** Génère un mot de passe à partir du secret en utilisant un compteur.
 * 
 * Cette fonction va générer un OTP à partir d'un secret et d'un compteur. <br/>
 * Pour deux couples (secret, compteur) identique la valeur retournée devrait 
 * être la même. <br/>
 * Il ne devrait pas être possible à partir de la valeur donné 
 * par un couple (secret, compteur) de déduire la valeur retournée par le couple
 * (secret, compteur + 1).<br/>
 * L'algorithme complet est disponible dans la 
 * <a href="https://tools.ietf.org/html/rfc4226">RFC 4226</a>.
 * @param[in] key le secret utilisé pour générer un OTP.
 * @param[in] counter le compteur permettant de générer un OTP.
 * @param[in] len la longueure de l'OTP a générer.
 * 
 * @pre secret != NULL.
 * @pre counter > 0.
 * @pre 6 <= len <= 8.
 * 
 * @return l'entier correspondant à l'OTP; -1 en cas d'échec
 */
int generateHOTP(secret key, long counter, int len);

#endif //TOTP_H
