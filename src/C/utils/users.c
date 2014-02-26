#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "users.h"
#include "secret.h"

#define SEPARATOR ":"
#define BUFFER_SIZE 256

int destroyOTPUser(char* usrname) {
    if (usrname == NULL) {
        return -1;
    }

    // Création d'un buffer pour accueillir le path vers le fichier de
    // l'utilisateur.
    char path[BUFFER_SIZE];
    if (snprintf(path, BUFFER_SIZE, "%s%s", OTPWD_PATH, usrname) < 0) {
        // Erreur non récupérable d'écriture dans un buffer.
        return -1;
    }

    // Suppression des données relatives à l'utilisateur.
    if (unlink(path) < 0) {
        // Erreur du système non récupérable à ce niveau.
        return -1;
    }

    // On indique que tout s'est bien passé à l'utilisateur.
    return 0;
}

int getOTPUser(char* usrname, otpuser* user) {

}

int updateOTPUser(otpuser* user) {

}
