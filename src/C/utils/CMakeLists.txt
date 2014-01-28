cmake_minimum_required (VERSION 2.6)
project (OTP)

add_definitions(--std=c99)

# Les deux libs statiques à générer
add_library(hotp hotp.c hotp.h utils.c utils.h secret.h)
add_library(totp totp.c totp.h utils.c utils.h secret.h)

# Création des tests
add_executable(test test.c utils.c utils.h secret.c secret.h)
target_link_libraries(test hotp totp)
