cmake_minimum_required (VERSION 2.6)

add_definitions(--std=c99 -Wall -Werror)

# Test la présence des lib utilisées.


# Les deux libs statiques à générer
add_library(otp STATIC otp.c otp.h utils.c utils.h secret.h)
add_library(users STATIC users.c secret.c)
#find_package(OpenSSL REQUIRED)
#IF(${CMAKE_BUILD_TYPE} MATCHES Debug)
 #   find_package(Check REQUIRED)
# Création des tests
add_executable(test-usr test-usr.c users.c secret.c)
target_link_libraries(test-usr check)

add_executable(test-gen test-gen.c utils.c utils.h secret.c secret.h)
target_link_libraries(test-gen otp crypto m)
#ENDIF(${CMAKE_BUILD_TYPE} MATCHES Debug)
