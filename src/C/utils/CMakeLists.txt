cmake_minimum_required (VERSION 2.6)
project (OTP)

add_definitions(--std=c99)
add_library(hotp hotp.c utils.c)
add_library(totp totp.c utils.c)
