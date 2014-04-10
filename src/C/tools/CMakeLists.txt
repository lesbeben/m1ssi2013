add_executable(otpasswd otpasswd.c)
target_link_libraries(otpasswd pam pam_misc)