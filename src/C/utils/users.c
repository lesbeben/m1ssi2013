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

#include "users.h"
#include "secret.h"

/** Répertoire destiné a accueillir les données d'authentification utilisateurs.
 *
 * Dans ce répertoire nous trouverons un fichier par utilisateur contenant:
 * <ul>
 *  <li> La méthode d'otp choisie par l'utilisateur. </li>
 *  <li> Le secret de l'utilisateur </li>
 *  <li> Les paramètres supplémentaires relatifs à la méthode d'otp </li>
 * </ul>
 */
#define OTPWD_PATH "/tmp/otpasswd"
#define SEPARATOR ":"
#define SWAP_FILE "/tmp/otpasswd~"
#define BUFFER_SIZE 1024


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

    // Premier token : le username.
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
        token = strtok_r(NULL, SEPARATOR, &saveptr);
        if (token == NULL) {
            free(user->username);
            destroySecret(user->passwd);
            return USR_ERR_USR_FILE;
        }
        user->params.hotp.count = atoi(token);
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
        if ((sprintf(line, "%s:%d:%s:%d:%d:%ld\n",user->username, user->method,
                     getHexRepresentation(user->passwd, bufferSecret,
                                          (2 * (user->passwd->length) + 1)),
                     user->otp_len, user->isBanned,
                     user->params.hotp.count)) < 0) {
            return USR_ERR_SYS;
        };
        break;

    case TOTP_METHOD :
        if ((sprintf(line, "%s:%d:%s:%d:%d:%ld:%d\n",user->username, user->method,
                     getHexRepresentation(user->passwd, bufferSecret,
                                          (2 * (user->passwd->length) + 1)),
                     user->otp_len, user->isBanned,
                     user->params.totp.tps, user->params.totp.delay)) < 0) {
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

int switchFile (char * from, char * to) {
    // Supprime le lien physique vers le fichier to
    if (unlink(to) == -1) {
        return USR_ERR_IO;
    }
    // Créér un lien physique pour le fichier to vers from
    if (link (from, to) == -1) {
        return USR_ERR_IO;
    }
    // Supprime le lien physique vers le fichier from
    if (unlink(from) == -1) {
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

/**
 * Vérifie que les permissions sur le fichiers sont conforme aux attentes.
 *
 * C'est à dire:
 *  -propriétaire: root
 *  -groupe: root
 *  -droits: 1700 <=> sticky bit + rwx------
 *
 * @return 0 si le fichier est correct, -1 sinon.
 */
int check_file_stats(FILE * f) {
    struct stat s;
    if (f == NULL) {
        return USR_ERR_PARAM;
    }
    if (fstat(fileno(f), &s) == -1) {
        // Impossible d'obtenir les informations du fichier.
        return USR_ERR_IO;
    }
// //     if (s.st_uid != 0) {
// //         // Le propriétaire n'est pas root.
// //         return -1;
// //     }
// //     if (s.st_gid != 0) {
// //         // Le propriétaire n'est pas root.
// //         return -1;
// //     }
    if ((s.st_mode & S_ISVTX) == 0) {
        // Le fichier n'a pas le sticky bit
        return USR_ERR_USR_FILE;
    }
    if ((s.st_mode &  (S_IRWXG | S_IRWXO)) != 0) {
        // Les droits des autres ne sont pas nuls.
        return USR_ERR_USR_FILE;
    }
    if ((s.st_mode & S_IRWXU) != S_IRWXU) {
        // Le propriétaire n'a pas tout les droits sur le fichier.
        return USR_ERR_USR_FILE;
    }
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
//     free(pwd);

    // Descripteur de fichier sur OTPWD_PATH.
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        return USR_ERR_IO;
    }
    if ((ret = check_file_stats(f)) != USR_SUCCESS) {
        return ret;
    }
    // Recherche de l'utilisateur dans le fichier
    while((ret = readLine(f, &usr)) == USR_SUCCESS) {
        if (!strcmp(usrname, usr.username)) {
            found = 1;
            *user = usr;
            break;
        }
    }


    if (fclose (f) != 0) {
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
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        int fd = open(OTPWD_PATH, O_WRONLY | O_CREAT | O_EXCL, S_IRWXU | S_ISVTX);
        if (fd < 0) {
            return USR_ERR_IO;
        }

        f = fdopen(fd, "w");
        if (f == NULL) {
            close(fd);
            return USR_ERR_IO;
        }
    }
    if ((ret = check_file_stats(f)) != USR_SUCCESS) {
        return ret;
    }

    // Descripteur de fichier temporaire.
    FILE * fw = fopen(SWAP_FILE, "w");
    if (fw == NULL) {
        fclose(f);
        return USR_ERR_IO;
    }

    // Mise en place des droits sur le nouveau fichier.
    if (fchmod(fileno(fw), S_ISVTX | S_IRWXU) == -1) {
        return USR_ERR_IO;
    }

    if ((ret = check_file_stats(f)) != USR_SUCCESS) {
        return ret;
    }

    if (flock(fileno(fw), LOCK_EX) == -1) {
        fclose(f);
        fclose(fw);
        return USR_ERR_IO;
    }
    // Recherche de l'utilisateur dans le fichier
    while((ret = readLine(f, &usr)) == USR_SUCCESS) {
        if (!strcmp(user->username, usr.username)) {
            if ((ret = writeLine (fw, user)) != USR_SUCCESS) {
                fclose(f);
                fclose(fw);
                return ret;
            }
            found = 1;
        } else {
            if ((ret = writeLine (fw, &usr)) != USR_SUCCESS) {
                fclose(f);
                fclose(fw);
                return ret;
            }
        }
    }

    if (!found && ret == NO_MORE_USERS) {
        if ((ret = writeLine(fw, user)) != USR_SUCCESS) {
            fclose(f);
            fclose(fw);
            return ret;
        }
    }

    if (fclose(f) != 0) {
        return USR_ERR_IO;
    }

    if ((ret = switchFile (SWAP_FILE, OTPWD_PATH)) != USR_SUCCESS) {
        return ret;
    }

    if (flock(fileno(fw), LOCK_UN) == -1) {
        fclose(fw);
        return USR_ERR_IO;
    }

    if (fclose(fw) != 0) {
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
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        return USR_ERR_IO;
    }

    if ((ret = check_file_stats(f)) != USR_SUCCESS) {
        return ret;
    }

    // Descripteur de fichier temporaire.
    FILE * fw = fopen(SWAP_FILE, "w");
    if (fw == NULL) {
        return USR_ERR_IO;
    }
    if (fchmod(fileno(fw), S_ISVTX | S_IRWXU) == -1) {
        return USR_ERR_IO;
    }

    if (flock(fileno(fw), LOCK_EX) == -1) {
        fclose(f);
        fclose(fw);
        return USR_ERR_IO;
    }

    // Recherche de l'utilisateur dans le fichier
    while((ret = readLine(f, &usr)) == USR_SUCCESS) {
        if (strcmp(usrname, usr.username)) {
            if ((ret = writeLine (fw, &usr)) != USR_SUCCESS) {
                fclose(f);
                fclose(fw);
                return ret;
            }
        }
    }

    if (fclose(f) != 0) {
        return USR_ERR_IO;
    }

    if ((ret = switchFile (SWAP_FILE, OTPWD_PATH)) != USR_SUCCESS) {
        return ret;
    }

    if (flock(fileno(fw), LOCK_UN) == -1) {
        fclose(fw);
        return USR_ERR_IO;
    }

    if (fclose(fw) != 0) {
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
//     free(pwd);

    FILE* users_base = fopen(OTPWD_PATH, "r");
    if (users_base == NULL) {
        struct stat s;
        if (stat(OTPWD_PATH, &s) == 0) {
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

