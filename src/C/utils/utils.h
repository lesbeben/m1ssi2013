#ifndef OTP_UTILS_H
#define OTP_UTILS_H
#include "secret.h"


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
char* HMAC_SHA1(unsigned long count, secret key, char * buffer);

/** Tronque un haché pour n'en retenir que 64 bits.
 * @param[in] hash le haché a tronquer.
 * @pre hash != NULL
 * @pre hash fait 160 bits.
 * 
 * @param[out] buffer le buffer qui contiendra la troncature.
 * @pre buffer != NULL
 * @pre buffer fait 32 bits (8 * 8 * sizeof(char)).
 * @post buffer contient la troncature du hash. Voir troncature RFC 4226.
 * 
 * @return l'adresse de buffer; NULL sinon
 */
char* truncate(char * hash, char * buffer);

/** Convertit un buffer en entier décimal.
 * @param[in] buffer le buffer à convertir.
 * @pre buffer != NULL
 * @pre buffer fait 64 bits.
 * 
 * @return l'entier correspondant à la valeur du buffer; -1 sinon.
 */
uint64_t convert(char * buffer); 

#endif