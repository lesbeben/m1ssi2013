#! /usr/bin/python

from hotp import Hotp_Generator
import getpass

def prompt(str):
    ok_to_me = False
    while (not ok_to_me):
        try:
            entree = input(str)
            out = int(entree)
            ok_to_me = True
        except ValueError:
            print("{0} n'est pas un entier".format(entree))
            ok_to_me = False
    return out


if __name__ == "__main__":
    secret = getpass.getpass("Secret: ")
    gen = Hotp_Generator(secret)
    ok_to_me = False
    gen.length = prompt("Taille: ")
    gen.count = prompt("Compteur: ")
    print("Mot de passe jetable: {0}".format(gen.get_otp()))

