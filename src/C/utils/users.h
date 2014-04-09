#ifndef USERS_H
#define USERS_H
#include "secret.h"
#include <stdint.h>

// Macros de code retour.
#define USR_SUCCESS                      0
#define USR_ERR_SYS                     -1
#define USR_ERR_IO                      -2
#define USR_ERR_USR_UKN                 -3
#define USR_ERR_USR_FILE                -4
#define USR_ERR_PARAM                   -5
#define NO_MORE_USERS                   -6

#define TOTP_METHOD 0
#define HOTP_METHOD 1

/** Union de tout les paramètres supplémentaire possibles.
 */
typedef union {
    struct {
        uint64_t count;  /**< Le compteur pour hotp. */
    } hotp;
    struct {
        uint64_t tps; /**< Date derniere authentification */
        int delay;
    } totp;
} otpopt;

/** Structure permettant de représenter un utilisateur en mémoire.
 */
typedef struct {
    char * username; /**< Le nom de l'utilisateur. */
    char method; /**< La méthode d'OTP de l'utilisateur.*/
    secret passwd; /**< Le secret de l'utilisateur. */
    char otp_len;      /**< La taille des otp à générer. */
    char isBanned;  /**< Indique si l'utilisateur à le droit de tenter 
                         de s'authentifier */
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