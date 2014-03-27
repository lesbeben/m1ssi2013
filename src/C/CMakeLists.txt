cmake_minimum_required (VERSION 2.6)

# Nom susceptible de changer.
project (OTP)

IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    ADD_DEFINITIONS(-fPIC) 
ENDIF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")

add_definitions(--std=c99 -Wall -Werror)
#Ajout du répertoire utils contenant les utilitaire/générateurs
add_subdirectory(utils)

link_directories(${CMAKE_SOURCE_DIR}/utils/)

set(CMAKE_SHARED_MODULE_PREFIX "")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

find_package(PAM)
add_library(pam_hotp MODULE pam_hotp.c)
# set_target_properties(pam_hotp PROPERTIES PREFIX "")
target_link_libraries(pam_hotp pam otp users m)
add_library(pam_totp MODULE pam_totp.c)
# set_target_properties(pam_totp PROPERTIES PREFIX "")
target_link_libraries(pam_totp pam otp users m)

set(PAM_LIB_DIR /usr/lib/security /lib/security /usr/lib64/security /lib64/security)
find_path(PAM_MODULES_PATH pam_unix.so HINTS ${PAM_LIB_DIR})

message("Répertoire de modules PAM détecté: ${PAM_MODULES_PATH}")

install(TARGETS pam_hotp LIBRARY DESTINATION ${PAM_MODULES_PATH})
install(TARGETS pam_totp LIBRARY DESTINATION ${PAM_MODULES_PATH})