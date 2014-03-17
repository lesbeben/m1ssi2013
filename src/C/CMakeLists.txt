cmake_minimum_required (VERSION 2.6)

# Nom susceptible de changer.
project (OTP)

IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    ADD_DEFINITIONS(-fPIC) 
ENDIF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")

add_definitions(--std=c99 -Wall -Werror)
#Ajout du répertoire utils contenant les utilitaire/générateurs
add_subdirectory(utils)

link_directories(utils/)

add_library(pam_hotp MODULE pam_hotp.c)
target_link_libraries(pam_hotp pam hotp)
add_library(pam_totp MODULE pam_totp.c)
target_link_libraries(pam_totp pam totp)