#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "secret.h"

#define SEPARATOR ":"
#define SWAP_FILE "/tmp/otpasswd~"
#define BUFFER_SIZE 1024

int getOTPUser(char* usrname, otpuser * user) {
    if (usrname == NULL) {
        return -1;
    }
    // Initialisation
    char line[BUFFER_SIZE];
    char *saveptr;
    char *token;
    int found = 0;

    // Descripteur de fichier sur OTPWD_PATH.
    FILE * f = fopen(OTPWD_PATH, "r");
    if (f == NULL) {
        return -1;
    }
    
    // Allocation du otpuser à retourner.
    otpuser * usr = user;
    if (usr == NULL) {
        return -1;
    }
    
    // Recherche de l'utilisateur dans le fichier
    while(fgets(line ,(BUFFER_SIZE + 1),f) != NULL){
        token = strtok_r(line, ":", &saveptr);
        if (strcmp(usrname, token)) {
            found = 1;
            // On remplie la struture otpuser
            int bufferLength = strlen(token);
            usr->username = (char *) malloc(sizeof(char) * bufferLength);
            strcpy((usr->username), usrname);
            token = strtok_r(NULL, ":", &saveptr);
            usr->method = atoi(token);
            token = strtok_r(NULL, ":", &saveptr);
            usr->passwd->length = strlen(token);
            usr->passwd->buffer = (char *) malloc(sizeof(char) * usr->passwd->length);
            strcpy((usr->passwd->buffer), token);
            token = strtok_r(line, ":", &saveptr);
            usr->params.count = atoi(token);
        }
    }
    
    fclose (f);
    
    if (found == 1) {
        return 0;
    } else {
        return -1;
    }
}

int updateOTPUser(otpuser* user) {
    return 0;
}

int DestroyOTPUser(char* usrname) {
    if (usrname == NULL) {
        //return NULL;
        return -1;
    }
    // Initialisation
    char line[BUFFER_SIZE];
    char *saveptr;
    char *token;

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
    while(fgets(line ,(BUFFER_SIZE + 1),f) != NULL){
        token = strtok_r(line, ":", &saveptr);
        if (!strcmp(usrname, token)) {
            if ((fputs (line, fw)) < 0);
       }
    }
    
    fclose(f);
    fclose(fw);
    
    //Faire pointer OTPWD_PATH vers SWAP_FILE

    return 0;
}