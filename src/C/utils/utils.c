#include <stdlib.h>
#include "utils.h"

char* HMAC_SHA1(unsigned long count, secret key, char * buffer) {
    // TODO
    return NULL;
}

char* truncate(char * hash, char * buffer) {
    // TODO
    return NULL;
}

int32_t convert(char* buffer) {
    if(buffer == NULL) {
        return -1;
    }
	return (int) (*buffer);
}
