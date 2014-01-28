#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "hotp.h"
#include "totp.h"
#include "utils.h"
#include "secret.h"

#define RESET 0
#define BRIGHT 1
#define DIM 2
#define UNDERLINE 3
#define BLINK 4
#define REVERSE	7
#define HIDDEN 8

#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

void textcolor(int attr, int fg, int bg);
void printOK(char *mess);
void printKO(char *mess);

int main(int argc, char * argv[]) {
    /*
     * Test secret.c
     */
    // Length 64
    printf("Testing createSecret and destroySecret function.\n");
    printf("Creating a secret of length 64.\n");
    secret s = createSecret(64);
    if (s == NULL || s->length != 64) {
        printKO("Incorrect size of secret.");
    }
    printf("Testing getLength\n");
    if (getLength(s) != 64) {
        printKO("Incorrect length for secret.");
    } else {
        printOK("Correct length.");
    }

    printf("Testing getHexRepresentation.\n");
    printf("Testing with NULL buffer and 0 length.\n");
    int length = 0;
    char *buff = NULL;
    if (getHexRepresentation(s, buff, length) != NULL) {
        printKO("Wrong representation for secret.");
    } else {
        printOK("Representation possibly right.");
    }
    printf("Testing with non NULL buffer and 0 length.\n");
    length = 0;
    buff = (char *) 12345;
    if (getHexRepresentation(s, buff, length) != NULL) {
        printKO("Wrong representation for secret.");
    } else {
        printOK("Representation possibly right.");
    }
    printf("Testing with NULL buffer and > 0 length.\n");
    length = 124;
    buff = NULL;
    if (getHexRepresentation(s, buff, length) != NULL) {
        printKO("Wrong representation for secret.");
    } else {
        printOK("Representation possibly right.");
    }
    printf("Normal test.\n");
    length = (2 * s->length + 1);
    buff = (char *) malloc(length * sizeof(char));
    if (strlen(getHexRepresentation(s, buff, length)) != length - 1) {
        printKO("Wrong representation for secret.");
    } else {
        printOK("Representation possibly right.");
    }
    printOK("Right for length 64.");
    printf("Destroying secret.\n");
    if (destroySecret(s) == -1) {
        printKO("Error while destroying. -1 returned.");
    }
    printOK("Passed. Trying for length -1.");

    // Length -1
    s = createSecret(-1);
    if (s != NULL) {
        printKO("Warning a value is associated.");
    }
    printOK("Pointer not set.");
    printf("Testing getLength\n");
    if (getLength(s) > 0) {
        printKO("Incorrect length for secret.");
    } else {
        printOK("Correct length.");
    }
    printf("Testing getHexRepresentation\n");
    if (strlen(getHexRepresentation(s, buff, length)) > 0) {
        printKO("Wrong representation for secret.");
    } else {
        printOK("Representation possibly right.");
    }
    printOK("Right for length -1.");
    printf("Destroying secret.\n");
    if (destroySecret(s) == -1) {
        printKO("Error while destroying. -1 returned.");
    }
    printOK("Passed.");
    free(buff);

    /*
     * Test utils.h
     * HMAC_SHA1
     */
    printf("Testing utils.c\n Testing HMAC\n");
    buff = (char *) malloc (20 * sizeof(char));
    char *buff1 = (char *) malloc (20 * sizeof(char));
    printf("Creating secret.\n");
    s = createSecret(1024);
    printOK("Secret created.");
    unsigned long c = 0;
    if (HMAC_SHA1(c, s, buff) == NULL) {
        printKO("Error while calculating HMAC -1 returned.");
    } else {
        printf("Testing length.\n");
        for (int i = 0; i < 20; i++) {
            buff[i];
        }
        printOK("Length >= 160 bits.");
    }
    printf("Calculating second HMAC.\n");
    if (HMAC_SHA1(c + 1, s, buff1) == NULL) {
        printKO("Error while calculating HMAC -1 returned.");
    } else {
        printf("Comparing values.\n");
        int cmp = 1;
        for (int i = 0; i < 20; i++) {
            if (buff[i] != buff1[i]) {
                cmp = 0;
            }
        }
        if (cmp == 1) {
            printKO("Values are equal. This should not hapen.");
        } else {
            printOK("Values are different.");
        }
    }
    printf("Testing with -1 as count.\n");
    if (HMAC_SHA1(-1, s, buff1) == NULL) printOK("Passed.");
    else printKO("Failed.");
    if (HMAC_SHA1(0, NULL, buff1) == NULL) printOK("Passed.");
    else printKO("Failed.");
    if (HMAC_SHA1(0, s, NULL) == NULL) printOK("Passed.");
    else printKO("Failed.");

    /* truncate */

    printf("Testing function truncate with bad cases.\n");
    free(buff1);
    buff1 = (char *) malloc (4 * sizeof(char));
    if (truncate(NULL, buff1) == NULL) {
        printOK("NULL hash correctly handled.");
    } else {
        printKO("NULL hash not correctly handled.");
    }

    free(buff1);
    buff1 = (char *) malloc (4 * sizeof(char));
    if (truncate(buff, NULL) == NULL) {
        printOK("NULL buffer correctly handled.");
    } else {
        printKO("NULL buffer not correctly handled.");
    }

    printf("Testing function truncate in normal circunstances.\n");
    free(buff1);
    buff1 = (char *) malloc (4 * sizeof(char));
    if (truncate(buff, buff1) == NULL) {
        printKO("Error while truncating NULL returned.");
    } else {
        printOK("Truncation correctly done.");
    }
    /* convert */
    uint64_t val;
    printf("Testing function convert.\n");
    val = convert(buff1);
    if (val == -1) {
        printKO("Error while converting. -1 returned.");
    } else {
        if (val < 0 || val >= 2147483648) { // 2^31
            printKO("Invalid output of conversion.");
        } else {
            printOK("Right value for conversion.");
        }
    }
    printf("Testing with wrong input.\n");
    if (convert(NULL) != -1) {
        printKO("NULL buffer badly handled.");
    } else {
        printOK("NULL buffer rightly handled.");
    }

    exit(0);
}

void textcolor(int attr, int fg, int bg) {
    char command[13];
    /* Command is the control command to the terminal */
    sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
    printf("%s", command);
}

void printOK(char *mess) {
    textcolor(BRIGHT, GREEN, BLACK);
    printf("[+] %s\n", mess);
    textcolor(RESET, WHITE, BLACK);
}

void printKO(char *mess) {
    textcolor(BRIGHT, RED, BLACK);
    printf("[-] %s\n", mess);
    textcolor(RESET, WHITE, BLACK);
}

