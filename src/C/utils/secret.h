#ifndef SECRET_H
#define SECRET_H

/** Type décrivant un secret.
 */
typedef struct {
    int length;
    char * buffer;
} secret_struct;

typedef secret_struct * secret;

/** Crée un "objet" secret.
 * @param[in] length la taille du secret en octets.
 * @pre la taille ne peut pas être négative.
 * 
 * @return L'adresse d'un nouveau secret. 
 */
secret createSecret(int length);

/** Transforme une chaîne hexadécimale en secret.
 * @param[in] buffer le buffer contenant la chaîne à transformer
 * @pre buffer se termine par un NULL byte.
 * @pre strlen(buffer) > 0
 * 
 * @return un secret initialisé avec le buffer; NULL en cas d'échec.
 */
secret hexToSecret(char * buffer);

/** Libères les ressources associés à un secret
 * @param[in] key le secret dont on veut libérer les ressources.
 * 
 * @return 0 si les ressources ont été libérées; -1 sinon.
 */
int destroySecret(secret key);

/** Retourne la longueur du secret.
 * @param[in] key le secret dont on veut connaître la longueur.
 * 
 * @return la longueur du secret; -1 en cas d'échec.
 */
int getLength(secret key);

/** Donne une représentation hexadécimale du secret.
 * @param[in] key le secret que l'on veut représenter.
 * @pre key != NULL
 * 
 * @param[out] buffer le buffer destiné à contenir la représentation du
 * secret.
 * @pre buffer != NULL.
 * @pre taille de buffer >= length.
 * @post buffer contient une représentation du secret.
 * @post strlen(buffer) <= length.
 * 
 * @param[in] length le nombre d'octet modifiables dans le buffer.
 * @pre length > 0
 * @pre length > (secret->length * 2)
 * 
 * @return l'adresse de buffer; NULL en cas d'échec.
 */
char * getRepresentation(secret key, char * buffer, int length);

#endif
