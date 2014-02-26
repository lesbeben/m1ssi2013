#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "users.h"
#include "secret.h"

#define SEPARATOR ":"
#define BUFFER_SIZE 1024

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

int destroyOTPUser(char* usrname) {
    char path[BUFFER_SIZE];
    if (snprintf(path, BUFFER_SIZE, "%s%s", OTPWD_PATH, usrname) < 0) {
        return -1;
    }
    unlink();
}

int getOTPUser(char* usrname, otpuser* user) {
    if (usrname == NULL) {
        return -1;
    }

    // Le fichier
    char path[BUFFER_SIZE];
    FILE * f;

    // La structure à remplir
    otpuser usr;
    usr.passwd = NULL;

    // Lecture du fichier
    if ((f = fopen (snprintf(path, BUFFER_SIZE, "%s%s", OTPWD_PATH, usrname), 'r')) == NULL) {
        return -1;
    } else {
        if (readLine(f, &usr)) {
            *user = usr;
        }
    }

    // Fermeture du fichier
    if (fclose (f) != 0) {
        return -1;
    }

    return 0;
}

int updateOTPUser(otpuser* user) {

}
