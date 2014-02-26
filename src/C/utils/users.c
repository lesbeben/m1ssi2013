#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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
    
    if (fgets(line ,(BUFFER_SIZE),f) == NULL) {
        
        return 0;
    }
    
    token = strtok_r(line, ":", &saveptr);
    int bufferLength = strlen(token);
    user->username = (char *) malloc(sizeof(char) * (bufferLength + 1)); 
    // Le null-byte de fin !
    strcpy((user->username), token);
    user->username[bufferLength] = 0; 
    // Le voila une chaîne de caractères se finit par un \0.
    token = strtok_r(NULL, ":", &saveptr);
    user->method = atoi(token);
    token = strtok_r(NULL, ":", &saveptr);
    user->passwd = hexToSecret(token);
    token = strtok_r(line, ":", &saveptr);
    user->params.count = atoi(token);
    return 1;
}


int writeLine (FILE *f, otpuser *user) {
    char line[BUFFER_SIZE];
    char bufferSecret[2 * (user->passwd->length) + 1];
    sprintf(line, "%s:%d:%s:%d\n",user->username, user->method, 
            getHexRepresentation(user->passwd, bufferSecret, 
                                 (2 * (user->passwd->length) + 1)),
            user->params.count);
    if (fputs (line, f) == EOF) {
        return -1;
    }
    return 0;
}

int switchFile (char * from, char * to) {
    if (unlink(to) == -1) {
        return -1;
    }
    if (link (from, to) == -1) {
        return -1;
    }
    if (unlink(from) == -1) {
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
        FILE * newF = fdopen(fd, "w");
        if (newF == NULL) {
            close(fd);
            return -1;
        }
        writeLine(newF, user);
        fclose(newF);
    } else {
        // Descripteur de fichier temporaire.
        FILE * fw = fopen(SWAP_FILE, "w");
        if (fw == NULL) {
            //return NULL;
            return -1;
        }
        // Recherche de l'utilisateur dans le fichier
        while(readLine(f, &usr)){
            if (!strcmp(user->username, usr.username)) {
                writeLine (fw, user);
                found = 1;
            } else {
                writeLine (fw, &usr);
            }
        }
        
        if (!found) {
            writeLine(fw, user);
        }
        
        if (fclose(f) != 0) {
            return -1;
        }
        if (fclose(fw) != 0) {
            return -1;
        }
        
        switchFile (SWAP_FILE, OTPWD_PATH);
        
        
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
    
    if (fclose(f) != 0) {
        return -1;
    }
    if (fclose(fw)  != 0) {
        return -1;
    }
    
    switchFile (SWAP_FILE, OTPWD_PATH);
    
    return 0;
}