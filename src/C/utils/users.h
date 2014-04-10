#ifndef USERS_H
#define USERS_H
#include "secret.h"
#include <stdint.h>
#include <sys/types.h>

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
        uint64_t tps; /**< Date derniere authentification.*/
        int delay;    /**< Décalage de l'utilisateur en nombre de quantum.*/
        int quantum; /**< La période de validité d'un otp pour l'utilisateur.*/
    } totp;
} otpopt;

/** Structure permettant de représenter un utilisateur en mémoire.
 */
typedef struct {
    char * username; /**< Le nom de l'utilisateur. */
    char method; /**< La méthode d'OTP de l'utilisateur.*/
    secret passwd; /**< Le secret de l'utilisateur. */
    char otp_len;      /**< La taille des OTP à générer. */
    char isBanned;  /**< Indique si l'utilisateur à le droit de tenter 
                         de s'authentifier */
    otpopt params; /**< Les paramètres optionnels pour l'authentification. */
} otpuser;

/** Obtient les informations de l'utilisateur dont le nom de connexion est
 *  "usrname".
 * 
 * Cette fonction doit aller lire le fichier des utilisateurs. Si il trouve une
 * ligne correspondant au nom de l'utilisateur il remplira alors la structure
 * passée paramètre. Sinon il indiquera l'erreur qu'il aura rencontré.
 * 
 * @param[in] usrname Le nom de l'utilisateur à rechercher.
 * 
 * @pre usrname != NULL
 * @pre usrname se termine par un NULL-byte.
 * 
 * @post user contient les données relative à l'utilisateur.
 *
 * @return USR_SUCCESS si tout s'est bien passé, une valeur négative sinon.
 */
int getOTPUser(const char * usrname, otpuser * user);

/** Enregistre les informations d'un utilisateur.
 * 
 * Cette fonction permet de mettre à jour les informations d'un utilisateur dans
 * le fichier OTPWD_PATH. Si ces informations n'existent pas alors elles seront
 * créées.
 * 
 * @param[in] user La structure utilisateur à enregistrer.
 * 
 * @pre user.username != NULL;
 * @post le fichier des utilisateurs a été mis à jour avec les information de 
 *       user
 * 
 * @return USR_SUCCESS si tout s'est bien passé une valeur négative sinon.
 */
int updateOTPUser(otpuser * user);

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

/** Supprime un utilisateur du fichier.
 * 
 * Cette fonction parcours le fichier des utilisateurs afin de supprimer 
 * l'entrée correspondant à l'utilisateur donné en paramètre.
 *
 * @param[in] usrname le nom de l'utilisateur à supprimer.
 *
 * @pre usrname != NULL
 *  *
 * @return retourne USR_SUCCESS, ou une valeur négative en cas d'erreur.
 *
 */
int removeOTPUser(char* usrname);

/** Libère les ressources occupées par la structure, ne libère PAS la structure.
 * 
 * Cette fonction libère les ressources occupées par la structure passée en 
 * paramètre. Elle ne doit pas libérer la structure elle même.
 * 
 * @param[in] user un pointeur vers la structure user à nettoyer.
 * 
 * @return USR_SUCCESS en cas de réussite, une valeur négative sinon.
 * 
 */
int resetOTPUser(otpuser * user);

/** Place un verrou sur le fichier OTPWD_PATH.

 * Si le fichier était vérouillé alors l'appel est bloquant
 * sinon l'appel verrouille le fichier et continue.
 * 
 * @return retourne USR_SUCCESS ou une valeur négative en cas d'erreur.
 *
 */
int lockFile();


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