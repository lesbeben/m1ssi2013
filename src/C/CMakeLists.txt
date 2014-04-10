cmake_minimum_required (VERSION 2.6)

# Nom susceptible de changer.
project (OTP C)


ADD_DEFINITIONS(-fPIC) 
add_definitions(--std=c99 -Wall -Werror)


# Pas de lib en préfixe des noms de modules.
set(CMAKE_MODULE_PATH ";${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")
find_package(PAM REQUIRED)

link_directories(${CMAKE_SOURCE_DIR}/utils/)
include_directories(${PAM_INCLUDE_DIR})

#Ajout du répertoire utils contenant les utilitaire/générateurs
add_subdirectory(utils)
add_subdirectory(tools)

add_library(pam_uotp MODULE pam_uotp.c options.c)
set_target_properties(pam_uotp PROPERTIES PREFIX "")
target_link_libraries(pam_uotp pam otp users m crypto)
remove_definitions(-fPIC)

install(TARGETS pam_uotp LIBRARY DESTINATION /lib/security)