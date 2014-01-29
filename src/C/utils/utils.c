#include "utils.h"

char* HMAC_SHA1(unsigned long count, secret key, char * buffer) {
    // TODO
}

char* truncate(char * hash, char * buffer) {
    // TODO
}

uint32_t convert(char* buffer) {
    if(buffer == NULL) {
        // buffer null
        return NULL;
	return (int) buffer;
}
