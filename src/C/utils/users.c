#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <malloc.h>
#include <errno.h>

#include "users.h"
#include "secret.h"

/** Fichier contenant les enregistrements des utilisateur.
 * 
 * Ce fichier contient une ligne par utilisateur avec:
 * <ul>
 *  <li> Le nom d'utilisateur </li>
 *  <li> La méthode d'authentification </li>
 *  <li> Le secret de l'utilisateur </li>
 *  <li> La longueur des otp a générer </li>
 *  <li> L'état de banissement de l'utilisateur </li>
 *  <li> 
 *      Les paramètres optionnels selon la méthode d'authentification <br/>
 *      Pour TOTP: <ul>
 *          <li>Le dernier compteur utilisé.</li>
 *          <li>Le décalage entre le token et le module en nombre de
 *              quantum.</li>
 *          <li>Le quantum, durée de validité d'un otp</li>
 *          <li>Date dernière authentification en UTC</li>
 *      </ul>
 *      Pour HOTP: <ul>
 *          <li>Le compteur pour générer le prochain OTP</li>
 *          <li>Date du dernier échec d'authentification</li>
 *          <li>Nombre d'échecs successifs</li>
 *      </ul>
 * </li>
 * </ul>
 */
#define OTPWD_PATH "/var/otpasswd"
#define SEPARATOR ":"
#define SWAP_FILE "/var/otpasswd~"
#define BUFFER_SIZE 1024
#define CPY_BUFFER_SIZE 4096

/*******************************************************************************
 *                                                                             *
 *                               UTILITAIRES                                   *
 *                                                                             *
 ******************************************************************************/

int readLine(FILE *f, otpuser *user) {
    // Initialisation
    char line[BUFFER_SIZE];
    char *token;
    char *saveptr;
    resetOTPUser(user);

    // Récupération de la ligne
    if (fgets(line ,(BUFFER_SIZE),f) == NULL) {
        return NO_MORE_USERS;
    }

    // Premier token : le nom d'utilisateur.
    token = strtok_r(line, SEPARATOR, &saveptr);
    if (token == NULL) {
        return USR_ERR_USR_FILE;
    }
    int bufferLength = strlen(token);
    user->username = (char *) malloc(sizeof(char) * (bufferLength + 1));
    if (user->username == NULL) {
        return USR_ERR_SYS;
    }
    strcpy((user->username), token);
    user->username[bufferLength] = 0;

    // Deuxième token : la méthode d'authentification
    token = strtok_r(NULL, SEPARATOR, &saveptr);
    if (token == NULL) {
        free(user->username);
        return USR_ERR_USR_FILE;
    }
    user->method = atoi(token);

    // Troisième token : le mot de passe en clair
    token = strtok_r(NULL, SEPARATOR, &saveptr);
    if (token == NULL) {
        free(user->username);
        return USR_ERR_USR_FILE;
    }
    user->passwd = hexToSecret(token);

    // Quatrième token : La longueur des otp.
    token = strtok_r(NULL, SEPARATOR, &saveptr);
    if (token == NULL) {
        free(user->username);
        destroySecret(user->passwd);
        return USR_ERR_USR_FILE;
    }
    user->otp_len = atoi(token);

    // Cinquième token : État de bannissement.
    token = strtok_r(NULL, SEPARATOR, &saveptr);
    if (token == NULL) {
        free(user->username);
        destroySecret(user->passwd);
        return USR_ERR_USR_FILE;
    }
    user->isBanned = atoi(token);

    // Dernier token : les paramètres optionnels
    switch (user->method) {
    case HOTP_METHOD :
        // Le compteur hotp
        token = strtok_r(NULL, SEPARATOR, &saveptr);
        if (token == NULL) {
            free(user->username);
            destroySecret(user->passwd);
            return USR_ERR_USR_FILE;
        }
        
        // Date dernière tentative
        user->params.hotp.count = atoi(token);
        token = strtok_r(NULL, SEPARATOR, &saveptr);
        if (token == NULL) {
            free(user->username);
            destroySecret(user->passwd);
            return USR_ERR_USR_FILE;
        }
        user->params.hotp.tplstauth = atoi(token);
        
        // NOmbre d'échecs
        token = strtok_r(NULL, SEPARATOR, &saveptr);
        if (token == NULL) {
            free(user->username);
            destroySecret(user->passwd);
            return USR_ERR_USR_FILE;
        }
        user->params.hotp.nbfail = atoi(token);
        break;

    case TOTP_METHOD :
        token = strtok_r(NULL, SEPARATOR, &saveptr);
        if (token == NULL) {
            free(user->username);
            destroySecret(user->passwd);
            return USR_ERR_USR_FILE;
        }
        user->params.totp.tps = atoi(token);
        token = strtok_r(NULL, SEPARATOR, &saveptr);
        if (token == NULL) {
            free(user->username);
            destroySecret(user->passwd);
            return USR_ERR_USR_FILE;
        }
        user->params.totp.delay = atoll(token);
        token = strtok_r(NULL, SEPARATOR, &saveptr);
        if (token == NULL) {
            free(user->username);
            destroySecret(user->passwd);
            return USR_ERR_USR_FILE;
        }
        user->params.totp.quantum = atoi(token);
        token = strtok_r(NULL, SEPARATOR, &saveptr);
        if (token == NULL) {
            free(user->username);
            destroySecret(user->passwd);
            return USR_ERR_USR_FILE;
        }
        user->params.totp.tplstauth = atoi(token);
        break;

    default :
        return USR_ERR_USR_FILE;
        break;
    }
    if (strtok_r(NULL,SEPARATOR, &saveptr) != NULL) {
        return USR_ERR_USR_FILE;
    }
    return USR_SUCCESS;
}

int writeLine (FILE *f, otpuser *user) {
    char line[BUFFER_SIZE];
    char bufferSecret[2 * (user->passwd->length) + 1];
    // On rempli le buffer avec données utilisateur
    switch (user->method) {
    case HOTP_METHOD :
        if ((snprintf(line,BUFFER_SIZE - 1 ,"%s:%d:%s:%d:%d:%ld:%ld:%d\n",user->username, user->method,
                     getHexRepresentation(user->passwd, bufferSecret,
                                          (2 * (user->passwd->length) + 1)),
                     user->otp_len, user->isBanned,
                     user->params.hotp.count,user->params.hotp.tplstauth, user->params.hotp.nbfail)) < 0) {
            return USR_ERR_SYS;
        };
        break;

    case TOTP_METHOD :
        if ((snprintf(line, BUFFER_SIZE - 1, "%s:%d:%s:%d:%d:%ld:%d:%d:%ld\n",user->username, user->method,
                     getHexRepresentation(user->passwd, bufferSecret,
                                          (2 * (user->passwd->length) + 1)),
                     user->otp_len, user->isBanned,
                     user->params.totp.tps, user->params.totp.delay,
                     user->params.totp.quantum, user->params.totp.tplstauth)) < 0) {
            return USR_ERR_SYS;
        };
        break;

    default :
        break;
    }
    // On écrit le buffer dans le fichier.
    if (fputs (line, f) == EOF) {
        return USR_ERR_IO;
    }
    return USR_SUCCESS;
}

/** Recopie le contenue du fichier @a src dans le fichier @a dst.
 * 
 * @param[in] dst Le fichier dans le quel il faut écrire.
 * @param[in] src Le fichier dans le quel il faut lire.
 * 
 * @return USR_SUCCESS en cas de succès, une autre macro en cas d'erreur.
 */
int fcpy (FILE * dst, FILE * src) {
    char buffer[CPY_BUFFER_SIZE];
    while (fgets(buffer, CPY_BUFFER_SIZE, src) != NULL) {
        if (fputs(buffer, dst) == EOF) {
            return USR_ERR_IO;
        }
    }
    if (ferror(src)) {
        return USR_ERR_IO;
    }
    return USR_SUCCESS;
}

int lockFile() {
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        return USR_ERR_IO;
    }
    if (flock(fileno(f), LOCK_EX) == -1) {
        fclose(f);
        return USR_ERR_IO;
    }
    fclose(f);
    return USR_SUCCESS;
}

int unlockFile() {
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        return USR_ERR_IO;
    }
    if (flock(fileno(f), LOCK_UN) == -1) {
        fclose(f);
        return USR_ERR_IO;
    }
    fclose(f);
    return USR_SUCCESS;
}

/*******************************************************************************
 *                                                                             *
 *                              FONCTIONS                                      *
 *                                                                             *
 ******************************************************************************/

int getOTPUser(const char* usrname, otpuser * user) {
    if (usrname == NULL || user == NULL) {
        return USR_ERR_PARAM;
    }

    // Initialisation
    int found = 0;
    otpuser usr;
    usr.username = NULL;
    usr.passwd = NULL;
    int ret = USR_SUCCESS;

    size_t bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1) {
        bufsize = 16384;
    }
    char * buf = malloc(bufsize);
    if (buf == NULL) {
        return USR_ERR_SYS;
    }
    
    // Test de l'existence de l'utilisateur sur le système.
    struct passwd pwd;
    struct passwd * result;
    ret = getpwnam_r(usrname, &pwd, buf, bufsize, &result);
    if (result == NULL) {
        free(buf);
        return USR_ERR_USR_UKN;
    }
    free(buf);

    // Descripteur de fichier sur OTPWD_PATH.
    FILE * user_file = fopen(OTPWD_PATH, "r");
    if (user_file == NULL) {
        return USR_ERR_IO;
    }

    // Recherche de l'utilisateur dans le fichier
    while((ret = readLine(user_file, &usr)) == USR_SUCCESS) {
        if (!strcmp(usrname, usr.username)) {
            found = 1;
            *user = usr;
            break;
        }
    }


    if (fclose (user_file) != 0) {
        return USR_ERR_IO;
    }

    // L'utilisateur n'a pas été trouvé dans le système.
    if (!found) {
        return USR_ERR_USR_UKN;
    }

    return USR_SUCCESS;
}

int updateOTPUser(otpuser* user) {
    if (user == NULL) {
        return USR_ERR_PARAM;
    }

    // Initialisation
    otpuser usr;
    usr.passwd = NULL;
    usr.username = NULL;
    int found = 0;
    int ret = USR_SUCCESS;
    size_t bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1) {
        bufsize = 16384;
    }
    char * buf = malloc(bufsize);
    if (buf == NULL) {
        return USR_ERR_SYS;
    }

    // Test de l'existence de l'utilisateur sur le système.
    struct passwd pwd;
    struct passwd * result;
    ret = getpwnam_r(user->username, &pwd, buf, bufsize, &result);
    if (result == NULL) {
        free(buf);
        return USR_ERR_USR_UKN;
    }
    free(buf);
//     free(pwd);

    // Descripteur de fichier sur OTPWD_PATH.
    FILE * user_file = fopen(OTPWD_PATH, "w+");
    if (user_file == NULL) {
        return USR_ERR_IO;
    }
    // Descripteur de fichier temporaire.
    FILE * updated_user_file = fopen(SWAP_FILE, "w+");
    if (updated_user_file == NULL) {
        fclose(user_file);
        return USR_ERR_IO;
    }

    // Mise en plase d'un verrou sur le fichier en cours de mise à jour.
    if (flock(fileno(updated_user_file), LOCK_EX) == -1) {
        fclose(user_file);
        fclose(updated_user_file);
        return USR_ERR_IO;
    }

    struct stat user_file_stats;
    if (fstat(fileno(user_file), &user_file_stats) != 0) {
        fclose(user_file);
        fclose(updated_user_file);
        return USR_ERR_IO;
    }
    if (fchmod(fileno(updated_user_file), user_file_stats.st_mode) != 0) {
        fclose(user_file);
        fclose(updated_user_file);
        return USR_ERR_IO;
    }
    if (fchown(fileno(updated_user_file), user_file_stats.st_uid, user_file_stats.st_gid) != 0) {
        fclose(user_file);
        fclose(updated_user_file);
        return USR_ERR_IO;
    }

    // Recherche de l'utilisateur dans le fichier
    while((ret = readLine(user_file, &usr)) == USR_SUCCESS) {
        if (!strcmp(user->username, usr.username)) {
            if ((ret = writeLine (updated_user_file, user)) != USR_SUCCESS) {
                fclose(user_file);
                fclose(updated_user_file);
                return ret;
            }
            found = 1;
        } else {
            if ((ret = writeLine (updated_user_file, &usr)) != USR_SUCCESS) {
                fclose(user_file);
                fclose(updated_user_file);
                return ret;
            }
        }
    }

    if (!found && ret == NO_MORE_USERS) {
        if ((ret = writeLine(updated_user_file, user)) != USR_SUCCESS) {
            fclose(user_file);
            fclose(updated_user_file);
            return ret;
        }
    }


    // Retour au début des deux fichiers.
    if (fseek(user_file, 0, SEEK_SET)) {
        fclose(user_file);
        fclose(updated_user_file);
        return USR_ERR_IO;
    }
    
    if (fseek(updated_user_file, 0, SEEK_SET)) {
        fclose(user_file);
        fclose(updated_user_file);
        return USR_ERR_IO;
    }
    
    ret = fcpy(user_file, updated_user_file);
    if (ret != USR_SUCCESS) {
        fclose(updated_user_file);
        fclose(user_file);
        return ret;
    }
    
    if (fclose(user_file) != 0) {
        fclose(updated_user_file);
        return USR_ERR_IO;
    }
    
    if (flock(fileno(updated_user_file), LOCK_UN) == -1) {
        fclose(updated_user_file);
        return USR_ERR_IO;
    }
    
    if (fclose(updated_user_file) != 0) {
        return USR_ERR_IO;
    }
    if (unlink(SWAP_FILE)) {
        return USR_ERR_IO;
    }

    return USR_SUCCESS;
}

int removeOTPUser(char* usrname) {
    if (usrname == NULL) {
        return USR_ERR_PARAM;
    }
    // Initialisation
    otpuser usr;
    usr.passwd = NULL;
    int ret;

    // Descripteur de fichier sur OTPWD_PATH.
    FILE * user_file = fopen(OTPWD_PATH, "r");
    if (user_file == NULL) {
        return USR_ERR_IO;
    }


    // Descripteur de fichier temporaire.
    FILE * updated_user_file = fopen(SWAP_FILE, "w");
    if (updated_user_file == NULL) {
        return USR_ERR_IO;
    }
    struct stat old_file_stats;
    if (fstat(fileno(user_file), &old_file_stats) != 0) {
        return USR_ERR_IO;
    }
    if (fchmod(fileno(updated_user_file), old_file_stats.st_mode) != 0) {
        return USR_ERR_IO;
    }
    if (fchown(fileno(updated_user_file), old_file_stats.st_uid, old_file_stats.st_gid) != 0) {
        return USR_ERR_IO;
    }

    if (flock(fileno(updated_user_file), LOCK_EX) == -1) {
        fclose(user_file);
        fclose(updated_user_file);
        return USR_ERR_IO;
    }

    // Recherche de l'utilisateur dans le fichier
    while((ret = readLine(user_file, &usr)) == USR_SUCCESS) {
        if (strcmp(usrname, usr.username)) {
            if ((ret = writeLine (updated_user_file, &usr)) != USR_SUCCESS) {
                fclose(user_file);
                fclose(updated_user_file);
                return ret;
            }
        }
    }

    // Retour au début des deux fichiers.
    if (fseek(user_file, 0, SEEK_SET)) {
        fclose(user_file);
        fclose(updated_user_file);
        return USR_ERR_IO;
    }
    
    if (fseek(updated_user_file, 0, SEEK_SET)) {
        fclose(user_file);
        fclose(updated_user_file);
        return USR_ERR_IO;
    }
    
    ret = fcpy(user_file, updated_user_file);
    if (ret != USR_SUCCESS) {
        fclose(updated_user_file);
        fclose(user_file);
        return ret;
    }
    
    if (fclose(user_file) != 0) {
        fclose(updated_user_file);
        return USR_ERR_IO;
    }
    
    if (flock(fileno(updated_user_file), LOCK_UN) == -1) {
        fclose(updated_user_file);
        return USR_ERR_IO;
    }
    
    if (fclose(updated_user_file) != 0) {
        return USR_ERR_IO;
    }
    
    if (unlink(SWAP_FILE)) {
        return USR_ERR_IO;
    }
    return USR_SUCCESS;
}

int userExists(const char* username) {
    // Test de l'existence de l'utilisateur sur le système.
    otpuser user;
    user.passwd = NULL;
    user.username = NULL;
    int ret = USR_SUCCESS;
    
    size_t bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1) {
        bufsize = 16384;
    }
    char * buf = malloc(bufsize);
    if (buf == NULL) {
        return USR_ERR_SYS;
    }
    
    // Test de l'existence de l'utilisateur sur le système.
    struct passwd pwd;
    struct passwd * result;
    ret = getpwnam_r(username, &pwd, buf, bufsize, &result);
    if (result == NULL) {
        free(buf);
        return USR_ERR_USR_UKN;
    }
    free(buf);

    FILE* users_base = fopen(OTPWD_PATH, "r");
    if (users_base == NULL) {
        struct stat s;
        if (stat(OTPWD_PATH, &s) != 0) {
            return 0;
        }
        return USR_ERR_IO;
    }
    while ((ret = readLine(users_base, &user)) == USR_SUCCESS) {
        if (strcmp(username, user.username) == 0) {
            if (fclose(users_base) != 0) {
                return USR_ERR_IO;
            }
            return 1;
        }
    }
    if (fclose(users_base) != 0) {
        return USR_ERR_IO;
    }
    return 0;
}

int resetOTPUser(otpuser* user) {
    if (user == NULL) {
        return USR_ERR_PARAM;
    }
    if (user->passwd != NULL) {
        if (destroySecret(user->passwd) != 0) {
            return USR_ERR_SYS;
        }
        user->passwd = NULL;
    }
    if (user->username != NULL) {
        free(user->username);
        user->username = NULL;
    }
    return USR_SUCCESS;
}

