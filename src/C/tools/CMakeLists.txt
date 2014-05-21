
# Compilation del 'utilitaire.
add_executable(otpasswd otpasswd.c)
target_link_libraries(otpasswd pam pam_misc)

# Installation de l'utilitaire de mise à jour de mdp.
install(TARGETS otpasswd RUNTIME DESTINATION /${CMAKE_INSTALL_BINDIR}
        PERMISSIONS 
        OWNER_EXECUTE OWNER_READ OWNER_WRITE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE SETUID SETGID)