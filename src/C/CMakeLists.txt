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

set(CMAKE_SHARED_MODULE_PREFIX "")
add_library(pam_hotp MODULE pam_hotp.c)
# set_target_properties(pam_hotp PROPERTIES PREFIX "")
target_link_libraries(pam_hotp pam hotp users m)
add_library(pam_totp MODULE pam_totp.c)
# set_target_properties(pam_totp PROPERTIES PREFIX "")
target_link_libraries(pam_totp pam totp users m)

find_path(PAM_MODULES_PATH pam_unix.so HINTS ${__UNIX_PATHS_INCLUDED})

message("Répertoire de modules PAM détecté: ${PAM_MODULES_PATH}")

install(TARGETS pam_hotp LIBRARY DESTINATION ${PAM_MODULES_PATH})
install(TARGETS pam_totp LIBRARY DESTINATION ${PAM_MODULES_PATH})