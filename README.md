m1ssi2013
=========

Projet Annuel de Master 1 SSI: Groupe 1 (OTP)

Rappel des commandes GIT
---

### Initialisation ###
  Cloner un dépôt distant:
  ```
  $ git clone url
  ```
  Lancer l'interface graphique:
  ```
  $ git gui
  ```

###Gestion des fichiers###
  Afficher l'état des fichiers:
  ```
  $ git status
  ```
  Afficher la liste des commit qui ont affecté un fichier:
  ```
  $ git log --follow nom-fichier
  ```
  Ajouter des fichiers pour le prochain commit:
  ```
  $ git add file [file]*
  ```
  Exclure des fichiers du prochain commit:
  ```
  $ git reset file [file]*
  ```
  Acter les changements sélectionnés:
  ```
  $ git commit
  ```
  Modification du commit précédent(en cas d'oubli d'un fichier):
  ```
  $ git commit --amend
  ```
  Annulation d'un ou plusieur commit:
  ```
  $ git revert SHA1 [SHA1]*
  ```
  Annulation (définitive) des changements depuis le commit précédent:
  ```
  $ git checkout -- fichier [fichier]*
  ```
###Gestion des branches ###
  Lister les branches:
  ```
  $ git branch
  ```
  Créer une nouvelle branche:
  ```
  $ git branch nom-branche
  ```
  Changer de branche:
  ```
  $ git checkout nom-branche
  ```
  Revenir en arrière dans la branche(lecture seule):
  ```
  $ git checkout SHA-COMMIT
  ```
  Retourner en bout de branche (suite à la commande précédente):
  ```
  $ git checkout nom-branche-courante
  ```
  Fusionner une branche dans la branche courante:
  ```
  $ git merge nom-branche
  ```
  Afficher le logs des commit sur la branche courante:
  ```
  $ git log
  ```
  Mettre de coté les changements non-commités et revenir au commit précédent:
  ```
  $ git stash
  ```
  Réappliquer les derniers changements mis dans la stash(ne dépile rien):
  ```
  $ git stash apply
  ```
  Vider la stash:
  ```
  $ git stash clear
  ```
  
###Partage et mise à jour###
  Mettre à jour la branche distante
  ```
  $ git push origin nom-branche
  ```
  Mettre à jour la branche locale en fusionnant la branche distante
  ```
  $ git pull
  ```

###Références###
  LE git pro book(en): http://git-scm.com/book
