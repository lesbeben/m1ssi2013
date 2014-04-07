cmake_minimum_required (VERSION 2.6)

# Nom susceptible de changer.
project (OTP C)


ADD_DEFINITIONS(-fPIC) 

add_definitions(--std=c99 -Wall -Werror)

#Ajout du répertoire utils contenant les utilitaire/générateurs
add_subdirectory(utils)

# Pas de lib en préfixe des noms de modules.
set(CMAKE_SHARED_MODULE_PREFIX "")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
find_package(PAM REQUIRED)

link_directories(${CMAKE_SOURCE_DIR}/utils/)
include_directories(${PAM_INCLUDE_DIR})

add_library(options STATIC options.c)

add_library(pam_uotp MODULE pam_uotp.c)
# set_target_properties(pam_uotp PROPERTIES PREFIX "")
target_link_libraries(pam_uotp pam otp users options m crypto)

install(TARGETS pam_uotp LIBRARY DESTINATION /lib/security)