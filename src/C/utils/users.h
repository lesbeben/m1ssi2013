#ifndef USERS_H
#define USERS_H
#include "secret.h"

#define TOTP_METHOD 0
#define HOTP_METHOD 1

/** Union de tout les paramètres supplémentaire possibles.
 */
typedef union {
    int count; /**< Le compteur pour hotp. */
    long tps; /**< Date derniere authentification */
} otpopt;

/** Structure permettant de représenter un utilisateur en mémoire.
 */
typedef struct {
    char * username; /**< Le nom de l'utilisateur. */
    int method; /**< La méthode d'OTP de l'utilisateur.*/
    secret passwd; /**< Le secret de l'utilisateur. */
    otpopt params; /**< Les paramètres optionels pour l'authentification. */
} otpuser;

/** Obtient les informations de l'utilisateur dont le login est usrname.
 * 
 * Cette fonction doit aller lire les informations dans le fichier 
 * correspondant à l'utilisateur puis remplir la structure passée en paramètre.
 * 
 * Si le fichier est introuvable on indiquera une erreur en retournant NULL.
 * @param[in] usrname Le nom de l'utilisateur à rechercher.
 * 
 * @pre usrname != NULL
 * @pre usrname se termine par un NULL-byte.
 *
 * @return 0 si tout s'est bien passé, -1 sinon.
 */
int getOTPUser(const char * usrname, otpuser * user);

/** Enregistre les informations d'un utilisateur.
 * 
 * Cette fonction permet de mettre à jour les informations d'un utilisateur dans
 * le fichier OTPWD_PATH. Si ces informations n'existent pas alors elles doivent
 * être créées.
 * 
 * @param[in] user La structure utilisateur à enregistrer.
 * @pre user.username != NULL;
 * 
 * @return -1 en cas d'erreur. 0 en cas de succès.
 */
int updateOTPUser(otpuser * user);

/** Libère les ressources associées à un utilisateur.
 *
 * Cette fonction permet d'encapsuler le type otpuser et de proposer
 * une libération des ressources simplifiée. Elle DOIT libérer toutes les 
 * ressources associées à l'utilisateur dans la RAM.
 *
 * @param[in] user un pointeur vers un otpuser.
 *
 * @pre user != NULL
 *
 * @return retourne 0, ou -1 en cas d'erreur.
 *
 */
int destroyOTPUser(char* usrname);

/** Place un verrou sur le fichier OTPWD_PATH.

 * Si le fichier était vérouillé alors l'appel est bloquant
 * sinon l'appel verrouille le fichier et continue.
 * 
 * @return retourne 0, ou -1 en cas d'erreur.
 *
 */
int lockFile();

/** Indique si un utilisateur existe.
 * 
 * Parcours le fichiers des utilisateurs pour trouver si un utilisateur existe.
 * si une entrée au nom de l'utilisateur existe alors la fonction retournera
 * 1, sinon elle retournera 0.
 * 
 * @param[in] username le nom de l'utilisateur recherché.
 * 
 * @return 1 si l'utilisateur existe, 0 si il n'existe pas ou -1 en cas 
 * d'erreur.
 */
int userExists(const char * username);

/** Enlève le verrou sur le fichier OTPWD_PATH.
 * 
 * Si le fichier était verouillé alors l'appel deverouille le
 * fichier et continue.
 * 
 * @return retourne 0, ou -1 en cas d'erreur.
 *
 */
int unlockFile();

#endif