CC=gcc
CFLAGS=-Wall -Werror -pedantic -std=c99
LDLIBS=-lotp -lcrypto -lm

otp_prng: otp_prng.o secret.o utils.o
	$(CC) -L../utils/ -o $@ $^ $(LDLIBS)

secret.o: ../utils/secret.c
	$(CC) -c $(CFLAGS) $<

utils.o: ../utils/utils.c ../utils/utils.h
	$(CC) -c $(CFLAGS) $<

otp_prng.o: otp_prng.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -rf *.o *~
