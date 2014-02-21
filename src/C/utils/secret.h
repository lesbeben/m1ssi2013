#ifndef SECRET_H
#define SECRET_H

/** \file
 * Fichier d'en-tête de fonctions permettant de manipuler des secrets binaires.
 */

/** Structure représentant un secret d'une longueur donnée en octet à l'aide
 * d'un pointeur sur un tableau d'octets.
 */
typedef struct {
    int length; /**< La taille du secret en octet, toujours supérieur à 0*/
    char * buffer; /**< Le pointeur vers les octets du tableau,
                        normalement non NULL. */
} secret_struct;

/** Type permettant de représenter un secret. Ce type permet d'«encapsuler»
 * la structure secret_struct.
 */
typedef secret_struct * secret;

/** Crée un "objet" secret rempli avec des length octets pseudo aléatoires.
 * 
 * Cette fonction se base sur le fichier /dev/urandom pour générer de 
 * l'aléatoire ce qui est plus rapide mais moins fiable que /dev/random.
 * @param[in] length la taille du secret en octets.
 * @pre la taille ne peut pas être négative.
 *
 * @return L'adresse d'un nouveau secret.
 */
secret createSecret(int length);

/** Crée un "objet" secret rempli avec des length octets aléatoire.
 * 
 * Cette fonction utilise le fichier /dev/random pour générer de l'aléatoire.<br/>
 * Elle est donc plus fiable mais beaucoup plus lente que createSecret.
 * @param[in] length la taille du secret en octets.
 * @pre la taille ne peut pas être négative.
 *
 * @return L'adresse d'un nouveau secret.
 */
secret createRandomSecret(int length);

/** Créée un secret à partir d'une chaîne de caractères héxadécimaux.
 * 
 * Cette fonctions va allouer un espace mémoire et convertir chaque paire de 
 * caractères héxadécimaux dans le buffer passé en paramètre en un octet qu'elle
 * gardera dans l'espace alloué. <br/>
 * Si il y a un caractère non héxadécimal dans la chaine buffer alors la
 * fonction retournera une erreur.
 * @param[in] buffer le pointeur sur le buffer contenant la chaîne à transformer
 * @pre buffer != NULL
 * @pre buffer[strlen(buffer) - 1] == 0.
 * @pre strlen(buffer) > 0
 * @pre pour 0 <= i < strlen(buffer):
 * @pre     '0' <= buffer[i] <= '9'
 * @pre  || 'a' <= buffer[i] <= 'f'
 * @pre  || 'A' <= buffer[i] <= 'F'
 *
 * @return un secret initialisé avec le buffer; NULL en cas d'échec.
 */
secret hexToSecret(char * buffer);

/** Créée un secret a partir d'une chaîne de caractères.
 * 
 * Cette fonction va utiliser les octets pointés par buffer comme nouveau 
 * secret. <br/>
 * Pour cela elle va allouer un nouvel espace en mémoire et recopier
 * les octets pointés par buffer.
 * @param[in] buffer le pointeur sur la chaîne de caractères.
 * @pre buffer != NULL
 * @pre strlen(buffer) > 0
 * @pre buffer[strlen(buffer) - 1] == 0
 *
 * @return un secret initialisé avec la chaîne de caractères pointée par buffer.
 */
secret textToSecret(char * buffer);

/** Libères les ressources associés à un secret.
 * 
 * Cette fonction désallouera les espace mémoires correspondant aux octets du 
 * secret ainsi qu'a la structure secret. <br/>
 * Par mesure de sécurité ces octets auront été mis à zéro avant libération.
 * @param[in] key le secret dont on veut libérer les ressources.
 * @pre key != NULL
 *
 * @return 0 si les ressources ont été libérées; -1 sinon.
 */
int destroySecret(secret key);

/** Retourne la longueur du secret.
 * 
 * @param[in] key le secret dont on veut connaître la longueur.
 * @pre key != NULL
 *
 * @return la longueur du secret; -1 en cas d'échec.
 */
int getLength(secret key);

/** Donne une représentation hexadécimale du secret.
 * 
 * Cette fonction va remplir le buffer passé en paramêtre avec length octets
 * afin de donner une représentation hexadécimale du secret. <br/>
 * Si length octet ne sont pas suffisant pour représenter le secret alors la 
 * fonction renverra une erreur.
 * 
 * @param[in] key le secret que l'on veut représenter.
 * @param[out] buffer le buffer destiné à contenir la représentation du
 * secret.
 * @param[in] length le nombre d'octet modifiables dans le buffer.
 *
 * @pre key != NULL
 * @pre buffer != NULL.
 * @pre taille de buffer >= length.
 * @pre length > 0
 * @pre length > (key->length * 2)
 *
 * @post buffer contient une représentation du secret.
 * @post strlen(buffer) <= length.
 *
 * @return l'adresse de buffer; NULL en cas d'échec.
 */
char * getHexRepresentation(secret key, char * buffer, int length);

/** Donne une représentation hexadécimale du secret.
 * 
 * Cette fonction va convertir chaque octet du secret en une paire de caractères
 * héxadécimaux et les inscrires dans le buffer.<br/>
 * Si la longueur passée en paramètre n'est pas suffisante alors la fonction
 * indiquera une erreur.
 * @param[in] key le secret que l'on veut représenter.
 * @param[in] length le nombre d'octet modifiables dans le buffer.
 * @param[out] buffer le buffer destiné à contenir la représentation du secret.
 *
 * @pre key != NULL
 * @pre buffer != NULL.
 * @pre taille de buffer >= length.
 * @pre length > 0
 * @pre length > key->length
 *
 * @post buffer contient une représentation du secret.
 * @post strlen(buffer) <= length.
 *
 * @return l'adresse de buffer; NULL en cas d'échec.
 */
char * getTextRepresentation(secret key, char * buffer, int length);

#endif
