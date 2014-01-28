#ifndef SECRET_H
#define SECRET_H

/** Type décrivant un secret.
 */
typedef struct {
    /** Le nombre d'octet du secret */
    int length;
    /** Un pointeur vers les octets pour représenter le secret */
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

/** Créée un secret à partir d'une chaîne de caractères héxadécimaux.
 * @param[in] buffer le pointeur sur le buffer contenant la chaîne à transformer
 * @pre buffer != NULL
 * @pre buffer[strlen(buffer) - 1] == 0.
 * @pre strlen(buffer) > 0
 * @pre pour 0 <= i < strlen(buffer):
 *      '0' <= buffer[i] <= '9' 
 *   || 'a' <= buffer[i] <= 'f'
 *   || 'A' <= buffer[i] <= 'F'
 * 
 * @return un secret initialisé avec le buffer; NULL en cas d'échec.
 */
secret hexToSecret(char * buffer);

/** Créée un secret a partir d'une chaîne de caractères.
 * @param[in] buffer le pointeur sur la chaîne de caractères.
 * @pre buffer != NULL
 * @pre strlen(buffer) > 0
 * @pre buffer[strlen(buffer) - 1] == 0
 * 
 * @return un secret initialisé avec la chaîne de caractères pointée par buffer. 
 */
secret textToSecret(char * buffer);

/** Libères les ressources associés à un secret
 * @param[in] key le secret dont on veut libérer les ressources.
 * @pre key != NULL
 * 
 * @return 0 si les ressources ont été libérées; -1 sinon.
 */
int destroySecret(secret key);

/** Retourne la longueur du secret.
 * @param[in] key le secret dont on veut connaître la longueur.
 * @pre key != NULL
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
 * 
 * @post buffer contient une représentation du secret.
 * @post strlen(buffer) <= length.
 * 
 * @param[in] length le nombre d'octet modifiables dans le buffer.
 * @pre length > 0
 * @pre length > (secret->length * 2)
 * 
 * @return l'adresse de buffer; NULL en cas d'échec.
 */
char * getHexRepresentation(secret key, char * buffer, int length);

/** Donne une représentation hexadécimale du secret.
 * @param[in] key le secret que l'on veut représenter.
 * @pre key != NULL
 * 
 * @param[out] buffer le buffer destiné à contenir la représentation du secret.
 * @pre buffer != NULL.
 * @pre taille de buffer >= length.
 * 
 * @post buffer contient une représentation du secret.
 * @post strlen(buffer) <= length.
 * 
 * @param[in] length le nombre d'octet modifiables dans le buffer.
 * @pre length > 0
 * @pre length > secret->length
 * 
 * @return l'adresse de buffer; NULL en cas d'échec.
 */
char * getTextRepresentation(secret key, char * buffer, int length);

#endif
