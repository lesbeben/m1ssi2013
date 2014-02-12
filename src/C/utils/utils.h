#ifndef OTP_UTILS_H
#define OTP_UTILS_H
#include "secret.h"
#include <inttypes.h>

#define HMAC_SHA1_LENGTH 20


/** Rempli le buffer passé en paramêtre avec le HMAC du secret.
 * @param[in] count le compteur passé à hmac pour générer un OTP.
 *
 * @param[in] key la clé secrète pour générer un OTP.
 * @pre key != NULL
 *
 * @param[out] buffer le buffer qui contiendra le résultat
 * @pre buffer != NULL
 * @pre buffer fait 160 (20 * 8 * sizeof(char)) bits.
 * @post buffer contient la valeur correspondant à
 *       SHA1(key ^ opad . SHA1(key ^ ipad . count))
 *       où ^ est le XOR bit à bits (^ en C)
 *       où . est la concaténation.
 *
 * @return l'adresse de buffer en cas de succès; NULL sinon.
 */
char* HMAC_SHA1(uint64_t count, secret key, char * buffer);

/** Extrait un entier d'un haché HMAC pour les générateur basés sur cette 
 * fonction. Cette fonction à pour but de remplacer truncate et convert. 
 * Plus KISS, pas de buffer de passage.
 * @param[in] hash le haché duquel il faut extraire l'OTP.
 * @pre hash != NULL.
 * @pre hash est un haché de HMAC_SHA1.
 *
 * @return un entier sur 31 bits à partir d'un haché; -1 en cas d'erreur.
 */
int32_t extractOTP(char* hash);

#endif
