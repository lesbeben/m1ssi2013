# Nom susceptible de changer.
project (OTP C)

cmake_minimum_required (VERSION 2.8.5)

# Flag de compilation communs.
add_definitions(--std=c99 -Wall -Werror -fPIC)

# Détection des répertoires d'installation.
include(GNUInstallDirs)


# Pas de lib en préfixe des noms de modules.
set(CMAKE_MODULE_PATH ";${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")
find_package(PAM REQUIRED)

link_directories(${CMAKE_SOURCE_DIR}/utils/)
include_directories(${PAM_INCLUDE_DIR})

#Ajout du répertoire utils contenant les utilitaire/générateurs
add_subdirectory(utils)
add_subdirectory(tools)

# Compilation du module PAM.
add_library(pam_uotp MODULE pam_uotp.c options.c)
set_target_properties(pam_uotp PROPERTIES PREFIX "")
target_link_libraries(pam_uotp pam otp users m crypto)

# Installation du module PAM.
install(TARGETS pam_uotp LIBRARY DESTINATION /${CMAKE_INSTALL_LIBDIR}/security)

# Installation de la configuration.
install(FILES conf/pam_otpasswd DESTINATION /etc/pam.d/ COMPONENT config 
        RENAME otpasswd)