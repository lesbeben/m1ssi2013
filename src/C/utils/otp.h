#ifndef HOTP_H
#define HOTP_H
#include "secret.h"
#include "utils.h"

// Macros code de retour
#define OTP_ERR_PARAM   -1
#define OTP_ERR_HMAC    -2
#define OTP_ERR_EXTRACT -3

/** \file
 * Fichier d'en-tête de fonctions pour générer des OTP basés sur la fonction
 * HMAC_SHA1. C'est le fichier d'en-tête pour toute la libhotp.a.
 */

/** Génère un mot de passe à partir du secret en utilisant un compteur.
 * 
 * Cette fonction va générer un OTP à partir d'un secret et d'un compteur. <br/>
 * Pour deux couples (secret, compteur) identique la valeur retournée devrait 
 * être la même. <br/>
 * Il ne devrait pas être possible de retrouver les parmètres à partir
 * de la sortie de la fonction.
 * 
 * L'algorithme complet est disponible dans la 
 * <a href="https://tools.ietf.org/html/rfc4226">RFC 4226</a>.
 * 
 * La variante TOTP requiert de passer le temps divisé par une durée de validité
 * en secondes comme compteur.
 * 
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
int generate_otp(secret key, long counter, int len);

#endif //TOTP_H
