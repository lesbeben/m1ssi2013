#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>

#include "users.h"
#include "secret.h"

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
    destroySecret(user->passwd);

    // Récupération de la ligne
    if (fgets(line ,(BUFFER_SIZE),f) == NULL) {
        return 0;
    }
    // Premier token : le username
    token = strtok_r(line, SEPARATOR, &saveptr);
    int bufferLength = strlen(token);
    user->username = (char *) malloc(sizeof(char) * (bufferLength + 1));
    strcpy((user->username), token);
    user->username[bufferLength] = 0;

    // Deuxième token : la méthode d'authentification
    token = strtok_r(NULL, SEPARATOR, &saveptr);
    user->method = atoi(token);

    // Troisième token : le mot de passe en clair
    token = strtok_r(NULL, SEPARATOR, &saveptr);
    user->passwd = hexToSecret(token);

    // Dernier token : le paramètre count
    token = strtok_r(NULL, SEPARATOR, &saveptr);
    user->params.count = atoi(token);

    return 1;
}

int writeLine (FILE *f, otpuser *user) {
    char line[BUFFER_SIZE];
    char bufferSecret[2 * (user->passwd->length) + 1];
    // On rempli le buffer avec données utilisateur
    if ((sprintf(line, "%s:%d:%s:%d\n",user->username, user->method,
                 getHexRepresentation(user->passwd, bufferSecret,
                                      (2 * (user->passwd->length) + 1)),
                 user->params.count)) < 0) {
        return -1;
    };
    // On écrit le buffer dans le fichier.
    if (fputs (line, f) == EOF) {
        return -1;
    }
    return 0;
}

int switchFile (char * from, char * to) {
    // Supprime le lien physique vers le fichier to
    if (unlink(to) == -1) {
        return -1;
    }
    // Créér un lien physique pour le fichier to vers from
    if (link (from, to) == -1) {
        return -1;
    }
    // Supprime le lien physique vers le fichier from
    if (unlink(from) == -1) {
        return -1;
    }
    return 0;
}

int lockFile() {
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        return -1;
    }
    if (flock(fileno(f), LOCK_EX) == -1) {
        fclose(f);
        return -1;
    }
    fclose(f);
    return 0;
}

int unlockFile() {
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        return -1;
    }
    if (flock(fileno(f), LOCK_UN) == -1) {
        fclose(f);
        return -1;
    }
    fclose(f);
    return 0;
}

/*******************************************************************************
 *                                                                             *
 *                              FONCTIONS                                      *
 *                                                                             *
 ******************************************************************************/

int getOTPUser(char* usrname, otpuser * user) {
    if (usrname == NULL || user == NULL) {
        return -1;
    }

    // Initialisation
    int found = 0;
    otpuser usr;
    usr.passwd = NULL;

    // Descripteur de fichier sur OTPWD_PATH.
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        return -1;
    }
    // Recherche de l'utilisateur dans le fichier
    while(readLine(f, &usr)) {
        if (!strcmp(usrname, usr.username)) {
            found = 1;
            *user = usr;
            break;
        }
    }

    if (fclose (f) != 0) {
        return -1;
    }
    if (found == 1) {
        return 0;
    } else {
        return -1;
    }
}

int updateOTPUser(otpuser* user) {
    if (user == NULL) {
        //return NULL;
        return -1;
    }
    // Initialisation
    otpuser usr;
    usr.passwd = NULL;
    int found = 0;

    // Descripteur de fichier sur OTPWD_PATH.
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        int fd = open(OTPWD_PATH, O_WRONLY | O_CREAT | O_EXCL, 0700);
        // Libère les ressources et on retourne une erreur.
        if (fd < 0) {
            // Cas où le fichier est présent sur le système où erreur I/O
            return -1;
        }
        // La on traite le cas sans erreur.
        f = fdopen(fd, "w");
        if (f == NULL) {
            close(fd);
            return -1;
        }
    }

    // Descripteur de fichier temporaire.
    FILE * fw = fopen(SWAP_FILE, "w");
    if (fw == NULL) {
        fclose(f);
        return -1;
    }

    if (flock(fileno(fw), LOCK_EX) == -1) {
        fclose(f);
        fclose(fw);
        return -1;
    }
    // Recherche de l'utilisateur dans le fichier
    while(readLine(f, &usr)) {
        if (!strcmp(user->username, usr.username)) {
            if (writeLine (fw, user) == -1) {
                fclose(f);
                fclose(fw);
                return -1;
            }
            found = 1;
        } else {
            if (writeLine (fw, &usr) == -1) {
                fclose(f);
                fclose(fw);
                return -1;
            }
        }
    }

    if (!found) {
        if (writeLine(fw, user) == -1) {
            fclose(f);
            fclose(fw);
            return -1;
        }
    }

    if (fclose(f) != 0) {
        return -1;
    }

    if (switchFile (SWAP_FILE, OTPWD_PATH) == -1) {
        return -1;
    }

    if (flock(fileno(fw), LOCK_UN) == -1) {
        fclose(fw);
        return -1;
    }

    if (fclose(fw) != 0) {
        return -1;
    }
    return 0;
}

int destroyOTPUser(char* usrname) {
    if (usrname == NULL) {
        return -1;
    }
    // Initialisation
    otpuser usr;
    usr.passwd = NULL;

    // Descripteur de fichier sur OTPWD_PATH.
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        return -1;
    }

    // Descripteur de fichier temporaire.
    FILE * fw = fopen(SWAP_FILE, "w");
    if (fw == NULL) {
        return -1;
    }
    if (flock(fileno(fw), LOCK_EX) == -1) {
        fclose(f);
        fclose(fw);
        return -1;
    }

    // Recherche de l'utilisateur dans le fichier
    while(readLine(f, &usr)) {
        if (strcmp(usrname, usr.username)) {
            if (writeLine (fw, &usr) == -1) {
                fclose(f);
                fclose(fw);
                return -1;
            }
        }
    }

    if (fclose(f) != 0) {
        return -1;
    }

    if (switchFile (SWAP_FILE, OTPWD_PATH) == -1) {
        return -1;
    }

    if (flock(fileno(fw), LOCK_UN) == -1) {
        fclose(fw);
        return -1;
    }

    if (fclose(fw) != 0) {
        return -1;
    }



    return 0;
}
