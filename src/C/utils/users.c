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

}
