m1ssi2013
=========

Projet Annuel de Master 1 SSI: Groupe 1 (OTP)

Rendu de projet :
---

### Dépendances module PAM: ###
Le module PAM utilise:
* glibc > 2.15
* openSSL > 1.0
* libpam > 1.0

Pour la compilation:
* gcc > 4.8
* cmake > 2.6

### Compilation et installation du module PAM: ###
```
git clone http://github.com/lesbeben/m1ssi2013/
cd m1ssi2013/src/C
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
make
sudo make install
```
