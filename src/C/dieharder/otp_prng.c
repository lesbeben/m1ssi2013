#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "../utils/otp.h"
#include "../utils/secret.h"

int main(int argc, char *argv[]) {
    /*
     * Parameters validation.
     */
    if (argc != 3) {
        printf(
            "Usage : %s seed nb len\n",
            argv[0]
            );
        printf("\tseed : The hexadecimal seed for generation\n");
        printf("\tnb : The number of values to generate.\n");
        exit(EXIT_SUCCESS);
    }
    long nb = strtol(argv[2], NULL, 10);
    if (nb <= 0) {
        printf("Invalid argument %s. nb must be a number.\n", argv[2]);
        exit(EXIT_SUCCESS);
    }
    long len = 8;
    /*
     * Seed validation
     */
    char *arg = argv[1];
    for (int i = 0; i < strlen(arg); i++) {
        char c[2];
        snprintf(c, 2, "%c", arg[i]);
        if (strcasestr("abcdef0123456789", c) == NULL) {
            printf("Seed must be an hexadecimal string.\n");
            exit(EXIT_SUCCESS);
        }
    }
    /*
     * Generation loop.
     */
    secret s = hexToSecret(argv[1]);
    int fd = open(
        "generated_bin",
        O_CREAT | O_TRUNC | O_WRONLY,
        S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Opening generated.bin");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nb; i++) {
        int value = generate_otp(s, i, len);
        /*
         * Write exclude extra 0 at the begining comming from the sign bit and
         * from the fact that otp < 10^8 whereas an int is 2^32.
         */
        if (write(fd, ((char *) &value), sizeof(int) - 1) == -1) {
            perror("writing");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }
    close(fd);
    exit(EXIT_SUCCESS);
}
