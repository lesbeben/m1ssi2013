/*******************************************************************************
 *                                                                             *
 *           Options: Récupérations des paramètres pour PAM                    * 
 *                                                                             *              *
 *                                                                             *
 ******************************************************************************/

#include <string.h>
#include <_pam_types.h>
#include <stdint.h>


#define USE_AUTH_TOK 0
#define DELAY_TOTP_AUTH 1
#define DELAY_HOTP_AUTH 2
#define NULL_OK 3
#define OPTIONS_ERROR -1
#define OPTIONS_SUCCESS 0
  
 typedef struct {
     char use_auth_tok; /**< Flag first_pass */
     union {
         uint64_t totp;
         uint64_t hotp;
     } delay;
     char null_ok; /**< Flag NULL OK */
 }modopt;
 
 /** Parse les arguments dans argv. 
 * 
 * Remplie la structure modopt passé en paramètre d'après le
 * contenu de l'argv.
 * 
 * @param[out] flag Structure modopt à remplir.
 * @param[in] argc Nombre d'arguments de argv.
 * @param[in] argv tableau des arguments à parser.
 *
 * @return Renvoie 0 en cas de succès, -1 sinon. 
 */
 int parse_options(pam_handle_t * pamh, modopt *flag, int argc, const char **argv);
 
 /** Defini l'option "field" à 1.
  * 
  * Passe l'option field à 1 dans la structure flag.
  * 
  * @param[out] flag Structure modopt que l'on souhaite modifier.
  * @param[in] field Le nom de l'option à activer.
  * @param[in] value La valeur à affecter.
  * 
  * @return Renvoie 0 en cas de succès, -1 sinon.
  */
 int set_opt(modopt *flag, int field, const char* value);
 
 /** Renvoie l'etat de l'option field.
  * 
  * Permet de récupérer l'état de l'option depuis la structure.
  * 
  * @param[in] flag Structure modopt que l'on souhaite consulter.
  * @param[in] field Le nom de l'option que l'on souhaite consulter.
  * 
  * @return Renvoie l'état de l'option field depuis la structure flag.
  */
 int get_opt(modopt *flag, int field);
