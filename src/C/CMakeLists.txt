cmake_minimum_required (VERSION 2.6)

# Nom susceptible de changer.
project (OTP C)


ADD_DEFINITIONS(-fPIC) 

add_definitions(--std=c99 -Wall -Werror)

#Ajout du répertoire utils contenant les utilitaire/générateurs
add_subdirectory(utils)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
find_package(PAM REQUIRED)

link_directories(${CMAKE_SOURCE_DIR}/utils/)
include_directories(${PAM_INCLUDE_DIR})

add_library(pam_hotp MODULE pam_hotp.c)
# set_target_properties(pam_hotp PROPERTIES PREFIX "")
target_link_libraries(pam_hotp pam otp users m)
add_library(pam_totp MODULE pam_totp.c)
# set_target_properties(pam_totp PROPERTIES PREFIX "")
target_link_libraries(pam_totp pam otp users m)

set(PAM_LIB_DIR /usr/lib/security /lib/security /usr/lib64/security /lib64/security)
find_path(PAM_MODULES_PATH pam_unix.so HINTS ${PAM_LIB_DIR})

if (${PAM_MODULES_PATH} MATCHES PAM_MODULES_PATH-NOTFOUND)
message(FATAL_ERROR "Impossible de déterminer le répertoire d'installation des modules PAM")
endif (${PAM_MODULES_PATH} MATCHES "PAM_MODULES_PATH-NOT_FOUND")
message("-- Répertoire d'installation des modules PAM: ${PAM_MODULES_PATH}")
install(TARGETS pam_hotp LIBRARY DESTINATION ${PAM_MODULES_PATH})
install(TARGETS pam_totp LIBRARY DESTINATION ${PAM_MODULES_PATH})