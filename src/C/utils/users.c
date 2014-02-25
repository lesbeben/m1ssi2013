#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    
    if (fgets(line ,(BUFFER_SIZE + 1),f) != NULL) {
        token = strtok_r(line, ":", &saveptr);
        int bufferLength = strlen(token);
        user->username = (char *) malloc(sizeof(char) * bufferLength);
        strcpy((user->username), token);
        token = strtok_r(NULL, ":", &saveptr);
        user->method = atoi(token);
        token = strtok_r(NULL, ":", &saveptr);
        user->passwd->length = strlen(token);
        user->passwd->buffer = (char *) malloc(sizeof(char) * user->passwd->length);
        strcpy((user->passwd->buffer), token);
        token = strtok_r(line, ":", &saveptr);
        user->params.count = atoi(token);
    } else {
        return -1;
    }
    return 0;
}

int writeLine (FILE *f, otpuser *user) {
    char line[BUFFER_SIZE];
    char bufferSecret[2 * (user->passwd->length) + 1];
    sprintf(line, "%s:%d:%s:%d",user->username, user->method, 
            getHexRepresentation(user->passwd, bufferSecret, 
                                 (2 * (user->passwd->length) + 1)),
            user->params.count);
    if (fputs (line, f) == EOF) {
        return -1;
    }
    return 0;
}

/*******************************************************************************
 *                                                                             *
 *                              FONCTIONS                                      *
 *                                                                             *
 ******************************************************************************/

int getOTPUser(char* usrname, otpuser * user) {
    if (usrname == NULL) {
        return -1;
    }
    if (user == NULL) {
        return -1;
    }

    // Initialisation
    int found = 0;
    otpuser usr;

    // Descripteur de fichier sur OTPWD_PATH.
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        return -1;
    }
    // Recherche de l'utilisateur dans le fichier
    while(readLine(f, &usr)){
        if (strcmp(usrname, usr.username)) {
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

    // Descripteur de fichier sur OTPWD_PATH.
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        //return NULL;
        return -1;
    }
    // Descripteur de fichier temporaire.
    FILE * fw = fopen(SWAP_FILE, "w");
    if (fw == NULL) {
        //return NULL;
        return -1;
    }
    // Recherche de l'utilisateur dans le fichier
    while(readLine(f, &usr)){
        if (strcmp(user->username, usr.username)) {
            writeLine (fw, user);
        } else {
            writeLine (fw, &usr);
        }
    }

    if (unlink(OTPWD_PATH) == -1) {
        return -1;
    }
    if (link (OTPWD_PATH, SWAP_FILE) == -1) {
        return -1;
    }
    if (unlink(SWAP_FILE) == -1) {
        return -1;
    }

    if (fclose(f) != 0) {
        return -1;
    }
    if (fclose(fw) != 0) {
        return -1;
    }

    return 0;
}

int DestroyOTPUser(char* usrname) {
    if (usrname == NULL) {
        //return NULL;
        return -1;
    }
    // Initialisation
    otpuser usr;

    // Descripteur de fichier sur OTPWD_PATH.
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        //return NULL;
        return -1;
    }
    // Descripteur de fichier temporaire.
    FILE * fw = fopen(SWAP_FILE, "w");
    if (fw == NULL) {
        //return NULL;
        return -1;
    }

    // Recherche de l'utilisateur dans le fichier
    while(readLine(f, &usr)){
        if (!strcmp(usrname, usr.username)) {
            writeLine (fw, &usr);
        }
    }

    if (unlink(OTPWD_PATH) == -1) {
        return -1;
    }
    if (link (OTPWD_PATH, SWAP_FILE) == -1) {
        return -1;
    }
    if (unlink(SWAP_FILE) == -1) {
        return -1;
    }

    if (fclose(f) != 0) {
        return -1;
    }
    if (fclose(fw)  != 0) {
        return -1;
    }

    return 0;
}