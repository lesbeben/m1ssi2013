#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"

#define SEPARATOR ":"
#define SWAP_FILE "/tmp/otpasswd~"
#define BUFFER_SIZE 1024

otpuser* getOTPUser(char* usrname) {
    if (usrname == NULL) {
        return NULL;
    }
    // Initialisation
    char line[BUFFER_SIZE];
    char *saveptr;
    char *token;
    int found = 0;

    // Descripteur de fichier sur OTPWD_PATH.
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        return NULL;
    }
    
    // Allocation du otpuser à retourner.
    otpuser usr = (otpuser) malloc(sizeof(*otpuser));
    if (usr == NULL) {
        return NULL;
    }
    
    // Recherche de l'utilisateur dans le fichier
    while(fgets(line ,(BUFFER_SIZE + 1),f) != NULL){
        token = strtok_r(line, ":", &saveptr);
        if (strcmp(usrname, token)) {
            found = 1;
            // On remplie la struture otpuser
            strcpy(usrname, usr.username);
            token = strtok_r(line, ":", &saveptr);
            usr.method = atoi(token);
            token = strtok_r(line, ":", &saveptr);
            strcpy(token, usr.passwd);
            token = strtok_r(line, ":", &saveptr);
            usr.params.count = atoi(token);
        }
    }
    
    fclose (f);
    
    if (found == 1) {
        return usr;
    } else {
        free(usr);
        return NULL;
    }
}

int updateOTPUser(otpuser* user) {
    return 0;
}

int freeOTPUser(otpuser* user) {
    return 0;
}