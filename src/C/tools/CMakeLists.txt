add_executable(otpasswd otpasswd.c)
target_link_libraries(otpasswd pam pam_misc)
install(TARGETS otpasswd RUNTIME DESTINATION bin
        PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE GROUP_READ
        GROUP_WRITE SETUID)