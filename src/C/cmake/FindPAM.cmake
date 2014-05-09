# - Try to find the PAM libraries
# Once done this will define
#
# PAM_FOUND - system has pam
# PAM_INCLUDE_DIR - the pam include directory
# PAM_LIBRARIES - libpam library

if (PAM_INCLUDE_DIR AND PAM_LIBRARY)
# Already in cache, be silent
set(PAM_FIND_QUIETLY TRUE)
endif (PAM_INCLUDE_DIR AND PAM_LIBRARY)

find_path(PAM_INCLUDE_DIR NAMES pam_appl.h 
    HINTS
    ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES}/security
    ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES}/pam)
find_library(PAM_LIBRARY pam)
find_library(DL_LIBRARY dl)

if (PAM_INCLUDE_DIR AND PAM_LIBRARY)
set(PAM_FOUND TRUE)
if (DL_LIBRARY)
set(PAM_LIBRARIES ${PAM_LIBRARY} ${DL_LIBRARY})
else (DL_LIBRARY)
set(PAM_LIBRARIES ${PAM_LIBRARY})
endif (DL_LIBRARY)

if (EXISTS ${PAM_INCLUDE_DIR}/pam_appl.h)
# darwin claims to be something special
set(HAVE_PAM_PAM_APPL_H 1)
endif (EXISTS ${PAM_INCLUDE_DIR}/pam_appl.h)

if (NOT EXISTS ${PAM_INCLUDE_DIR}/_pam_macros.h)
message(FATAL_ERROR "Impossible de trouver _pam_macros.h")
endif (NOT EXISTS ${PAM_INCLUDE_DIR}/_pam_macros.h)

if (NOT EXISTS ${PAM_INCLUDE_DIR}/pam_modules.h)
message(FATAL_ERROR "Impossible de trouver pam_modules.h")
endif (NOT EXISTS ${PAM_INCLUDE_DIR}/pam_modules.h)

if (NOT EXISTS ${PAM_INCLUDE_DIR}/pam_ext.h)
message(FATAL_ERROR "Impossible de trouver pam_ext.h")
endif (NOT EXISTS ${PAM_INCLUDE_DIR}/pam_ext.h)

set(PAM_MESSAGE_CONST ${PAM_MESSAGE_CONST} CACHE BOOL "PAM expects a 
conversation function with const pam_message")

endif (PAM_INCLUDE_DIR AND PAM_LIBRARY)

if (PAM_FOUND)
if (NOT PAM_FIND_QUIETLY)
message(STATUS "Found PAM: ${PAM_LIBRARIES}")
endif (NOT PAM_FIND_QUIETLY)
else (PAM_FOUND)
if (PAM_FIND_REQUIRED)
message(FATAL_ERROR "PAM was not found")
endif(PAM_FIND_REQUIRED)
endif (PAM_FOUND)

mark_as_advanced(PAM_INCLUDE_DIR PAM_LIBRARY DL_LIBRARY PAM_MESSAGE_CONST)