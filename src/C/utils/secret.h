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
 * @return L'adresse d'un nouveau secret. 
 */
secret createSecret(int length);

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

/** Retourne une représentation "human readable" du secret.
 * @param[in] key le secret que l'on veut représenter.
 * @pre key != NULL
 * 
 * @param[out] buffer le buffer destiné à contenir la représentation du
 * secret.
 * @pre buffer != NULL.
 * @pre taille de buffer >= length.
 * @post buffer contient une représentation .
 * @post strlen(buffer) <= length.
 * 
 * @param[in] length le nombre d'octet modifiables dans le buffer.
 * @pre length > 0
 * @pre buffer doit avoir été alloué et contenir length octets.
 * 
 * @return l'adresse de buffer; NULL en cas d'échec.
 */
char * getRepresentation(secret key, char * buffer, int length);

#endif
