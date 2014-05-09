#ifndef OTP_UTILS_H
#define OTP_UTILS_H
#include "secret.h"
#include <inttypes.h>

#define HMAC_SHA1_LENGTH 20

/** \file
 * Fichier d'en-tête de fonctions outils pour générer des OTP avec HMAC.
 */

/** Remplit le buffer passé en paramêtre avec le HMAC du secret.
 * D'après l'algorithme donné par la RFC 2104
 * @param[in] count le compteur passé à hmac pour générer un OTP.
 * @param[in] key la clé secrète pour générer un OTP.
 * @param[out] buffer le buffer qui contiendra le résultat
 *
 *
 * @pre key != NULL
 * @pre buffer != NULL
 * @pre buffer fait 160 (20 * 8 * sizeof(char)) bits.
 * @post buffer contient la valeur correspondant à
 *       SHA1(key ^ opad . SHA1(key ^ ipad . count))
 *       <br/>où ^ est le XOR bit à bits (^ en C)
 *       <br/>et . est la concaténation.
 *
 * @return l'adresse de buffer en cas de succès; NULL sinon.
 */
char* HMAC_SHA1(uint64_t count, secret key, char * buffer);

/** Extrait un entier d'un haché HMAC pour les générateur basés sur cette 
 * fonction. D'après la fonction de troncature de la rfc 4226.
 * @param[in] hash le haché duquel il faut extraire l'OTP.
 * @pre hash != NULL.
 * @pre hash est un haché de HMAC_SHA1. sur 20 octets.
 * 
 * @return un entier sur 31 bits à partir d'un haché; -1 en cas d'erreur.
 */
int32_t extractOTP(char* hash);

#endif
