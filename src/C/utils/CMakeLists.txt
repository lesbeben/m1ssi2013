cmake_minimum_required (VERSION 2.6)
project (OTP)

add_definitions(--std=c99 -Wall -Werror)

# Test la présence des lib utilisées.
find_library(${__UNIX_PATHS_INCLUDED} crypto)
find_library(${__UNIX_PATHS_INCLUDED} m)
find_library(${__UNIX_PATHS_INCLUDED} check)

# Les deux libs statiques à générer
add_library(hotp STATIC hotp.c hotp.h utils.c utils.h secret.h)
add_library(totp STATIC totp.c totp.h utils.c utils.h secret.h)
add_library(users STATIC users.c secret.c)

# Création des tests
add_executable(test-usr test-usr.c users.c secret.c)
target_link_libraries(test-usr check)

add_executable(test-gen test-gen.c utils.c utils.h secret.c secret.h)
target_link_libraries(test-gen hotp totp crypto m)
