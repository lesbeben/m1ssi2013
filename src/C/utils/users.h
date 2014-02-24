#ifndef USERS_H
#define USERS_H
#include "secret.h"

#define TOTP_METHOD 0
#define HOTP_METHOD 1

/** Le nom du fichier où sont stockées les informations nécessaire à 
 * l'authentification de chaque utilisateurs. <br/>
 * Ce fichier contient une ligne par utilisateur avec:
 * <ul>
 *  <li>Le login de l'utilisateur.</li>
 *  <li>La méthode d'OTP choisie.</li>
 *  <li>Le secret de l'utilisateur au format hexadécimal.</li>
 *  <li>Les éventuels paramètres supplémentaires pour la génération d'otp.</li>
 * </ul>

 * @todo Réfléchir à la possibilité de créer en tant qu'utilisateur.
 * @todo Changer la valeur d'OTPWD_DIR pour la mise en production.
 */
#define OTPWD_PATH "/tmp/otpasswd"

/** Union de tout les paramètres supplémentaire possibles.
 */
typedef union {
    int count; /**< Le compteur pour hotp. */
} otpopt;

/** Structure permettant de représenter un utilisateur en mémoire.
 */
typedef struct {
    char * username; /**< Le nom de l'utilisateur. */
    int method; /**< La méthode d'OTP de l'utilisateur.*/
    secret passwd; /**< Le secret de l'utilisateur. */
    otpopt params; /**< Les paramètres optionels pour l'authentification. */
} otpuser;

/** Obtient les information de l'utilisateur dont le login est usrname.
 * 
 * Cette fonction doit aller lire les information dans le fichier 
 * correspondant à l'utilisateur pour allouer puis
 * remplir une structre et retourner un pointeur sur celle ci.<br/>
 * Si le fichier est introuvable on indiquera une erreur en retournant NULL.
 * @param[in] usrname Le nom de l'utilisateur à rechercher.
 * 
 * @pre usrname != NULL
 * @pre usrname se termine par un NULL-byte.
 *
 * @return Un pointeur sur une structure utilisateur; NULL en cas d'erreur.
 */
otpuser * getOTPUser(char * usrname);

/** Enregistre les information d'un utilisateur.
 * 
 * Cette fonction permet de mettre à jour les information d'un utilisateur dans
 * le fichier OTPWD_PATH. Si ces informations n'existent pas alors elles doivent
 * être créée.
 * 
 * @param[in] user La structure utilisateur à enregistrer.
 * @pre user.username != NULL;
 * 
 * @return -1 en cas d'erreur.
 */
int updateOTPUser(otpuser * user);

/** Libère les ressources associés à un utilisateur.
 * 
 * Cette fonction permet d'encapsuler le type otpuser et de proposer
 * une libération des ressources simplifiée. Elle DOIT libérer toutes les 
 * ressources associées à l'utilisateur dans la RAM.
 * 
 * @param[in] user un pointeur vers un otpuser.
 * 
 * @pre user != NULL
 * 
 * @return -1 en cas d'erreur, une valeur positive sinon.
 * 
 */
int freeOTPUser(otpuser * user);