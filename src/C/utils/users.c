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

int destroyOTPUser(char* usrname) {
    char path[BUFFER_SIZE];
    if (snprintf(path, BUFFER_SIZE, "%s%s", OTPWD_PATH, usrname) < 0) {
        return -1;
    }
    unlink();
}

int getOTPUser(char* usrname, otpuser* user) {

}

int updateOTPUser(otpuser* user) {
    if (user == NULL) {
        return -1;
    }
    char path[BUFFER_SIZE];
    if (snprintf(path, BUFFER_SIZE, "%s%s", OTPWD_PATH, user->username) < 0) {
        return -1;
    }
    // Descripteur de fichier.
    FILE * f = fopen(path, "r");
    if (f == NULL) {
        return -1;
    }
    
    char line[BUFFER_SIZE];
    char bufferSecret[2 * (user->passwd->length) + 1];
    sprintf(line, "%s:%d:%s:%d\n",user->username, user->method, 
            getHexRepresentation(user->passwd, bufferSecret, 
                                 (2 * (user->passwd->length) + 1)),
            user->params.count);
    
    if (fputs (line, f) == EOF) {
        return -1;
    }
    
    if (fclose (f) != 0) {
        return -1;
    }
    
    return 0;
}
