#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

#include "hotp.h"
#include "totp.h"
#include "utils.h"
#include "secret.h"

#define RESET 0
#define BRIGHT 1
#define DIM 2
#define UNDERLINE 3
#define BLINK 4
#define REVERSE 7
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
    //Create secret & destroy
    printf("Testing createSecret and destroySecret function.\n");
    printf("Creating a secret of length 64.\n");
    secret s = createSecret(64);
    if (s == NULL) {
        printKO("secret shouldn't be NULL when size is 64.");
    } else {
        printOK("secret successfuly allocated.");
    }
    if ( s->length != 64) {
        printKO("Incorrect size of secret.");
    } else {
        printOK("secret size properly set.");
    }
    //get length
    printf("Testing getLength\n");
    if (getLength(s) != 64) {
        printKO("getLength should have returned 64.");
    } else {
        printOK("Correct length.");
    }
    //Get hexRepresentation
    printf("Testing getHexRepresentation.\n");
    printf("Testing with NULL buffer and 0 length.\n");
    int length = 0;
    char *buff = NULL;
    if (getHexRepresentation(s, buff, length) != NULL) {
        printKO("getHexRepresentation should have returned NULL.");
    } else {
        printOK("getHexRepresentation returned NULL.");
    }
    printf("Testing with non NULL buffer and 0 length.\n");
    length = 0;
    buff = (char *) 12345;
    if (getHexRepresentation(s, buff, length) != NULL) {
        printKO("getHexRepresentation should have returned NULL.");
    } else {
        printOK("getHexRepresentation returned NULL.");
    }
    printf("Testing with NULL buffer and > 0 length.\n");
    length = 124;
    buff = NULL;
    if (getHexRepresentation(s, buff, length) != NULL) {
        printKO("getHexRepresentation should have returned NULL.");
    } else {
        printOK("getHexRepresentation have returned NULL.");
    }

    printf("Normal test.\n");
    length = (2 * s->length + 1);
    buff = (char *) malloc(length * sizeof(char));
    if (strlen(getHexRepresentation(s, buff, length)) != (2 * s->length)) {
        printKO("Wrong representation for secret.");
        printf("Got : %s of length %d instead of %d\n",
               buff, (int) strlen(buff), 2 * s->length);
    } else {
        printOK("Representation possibly right.");
    }
    //hextosecret
    printf("Testing hexToSecret.\n");
    char *hex = "2a10f5dd4e27f20a";
    secret s2 = hexToSecret(hex);
    if ((s2->length) != (strlen(hex) / 2)) {
        printKO("Inconsistant length for secret.");
        printf("%d expected. Got %d.\n",
               (int) strlen(hex) / 2,
               (int) s2->length);
    } else {
        printOK("Right length for secret.");
    }
    //coherence
    printf("Testing coherence between hexToSecret and getHexRepresentation.\n");
    char rep[21];
    getHexRepresentation(s2, rep, 21);
    if (strcmp(rep, hex) == 0) {
        printOK("hexToSecret coherent with getHexRepresentation.");
    } else {
        printKO("hexToSecret not coherent with getHexRepresentation.");
        printf("%s waited.\n", hex);
        printf("got %s\n", rep);
    }

    printOK("Right for length 64.");
    printf("Destroying secret.\n");
    if (destroySecret(s) == -1) {
        printKO("Error while destroying. -1 returned.");
    } else {
        printOK("Secret successfuly destroyed");
    }
    printOK("Passed.");

    printf("Trying for length -1.\n");
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
    if (getHexRepresentation(s, buff, length) != NULL) {
        printKO("getHexRepresentation should return NULL when key is NULL");
    } else {
        printOK("getHexRepresentation has returned NULL");
    }
    printOK("Right for length -1.");
    printf("Destroying secret.\n");
    if (destroySecret(s) == -1) {
        printOK("destroySecret has returned -1.");
    } else {
        printKO("destroySecret should have returned -1.");
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
        char bidon;
        for (int i = 0; i < 20; i++) {
            bidon = buff[i];
            bidon++;
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
    printf("NULL as secret.\n");
    if (HMAC_SHA1(0, NULL, buff1) == NULL) printOK("Passed.");
    else printKO("Failed.");
    printf("Testing with NULL as buffer.\n");
    if (HMAC_SHA1(0, s, NULL) == NULL) printOK("Passed.");
    else printKO("Failed.");

    //Extract OTP !
    char buffExtr[] = {
        23, 51, 2, 43, 114, 57, 63, 71, 48, 9,
        19, 108, 27, 56, 105, 74, 33, 22, 51,10
    };
    uint32_t res = extractOTP(buffExtr);
    if (res != 721564439) {
        printKO("Incorrect value for extract OTP.");
        printf("Expected : 291; Received : %d\n", res);
    } else {
        printOK("Right value for extract OTP.");
        printf("Received : %d\n", res);
    }

    /* Tests hotp.c*/
    printf("Testing HOTP calculation.\n");
    s = hexToSecret("AABBCCDDEEFF");
    int otp, otp2;
    if ((otp = generateHOTP(s, 0, 6)) == -1) {
        printKO("Error while executing. -1 returned.");
    } else {
        if (0 > otp || otp > 999999) {
            printKO("Invalid length for otp 6 wanted.");
        } else {
            printOK("Correct length for otp. OTP");
            printf("OTP : %d\n", otp);
        }
    }
    if ((otp2 = generateHOTP(s, 1, 6)) == otp) {
        printKO("Two successive valus are equal. This should not happen.");
    } else {
        printOK("Two successive values are differents.");
        printf("OTP2 : %d\n", otp2);
    }

    if (generateHOTP(NULL, 0, 7) != -1)
        printKO("NULL secret not correctly handled");
    if (generateHOTP(s, 0, -1) != -1)
        printKO("-1 length not correctly handled");
    if (generateHOTP(s, 0, 99) != -1)
        printKO("99 length not correctly handled");

    /* Testing TOTP calculation. */
    printf("Testing TOTP calculation.\n");
    if ((otp = generateTOTP(s, 1, 6)) == -1) {
        printKO("Error while executing. -1 returned.");
    } else {
        if (0 > otp || otp > 999999) {
            printKO("Invalid length for otp 6 wanted.");
        } else {
            printOK("Correct length for otp.");
        }
    }
    sleep(2);
    if (generateTOTP(s, 1, 6) == otp) {
        printKO("Two successive valus are equal. This should not happen.");
    } else {
        printOK("Two successive values are differents.");
    }

    if (generateTOTP(NULL, 0, 7) != -1)
        printKO("NULL secret not correctly handled");
    if (generateTOTP(s, -1, 7) != -1)
        printKO("bad quantum not correctly handled");
    if (generateTOTP(s, 0, -1) != -1)
        printKO("-1 length not correctly handled");
    if (generateTOTP(s, 0, 99) != -1)
        printKO("99 length not correctly handled");

    destroySecret(s);
    exit(EXIT_SUCCESS);
}

void textcolor(int attr, int fg, int bg) {
    char command[13];
    /* Command is the control command to the terminal */
    snprintf(command, 13, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
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
