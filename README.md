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
  Ajouter des fichiers pour le prochain commit:
  ```
  $ git add file [file]*
  ```
  Exclure des fichiers du prochain commit:
  ```
  $ git reset file [file]*
  ```
  Acter les changements sélectionner:
  ```
  $ git commit
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
  Fusionner une branche dans la branche courante:
  ```
  $ git merge nom-branche
  ```
  Afficher le logs des commit sur la branche courante:
  ```
  $ git log
  ```
  Mettre de coté les changements non-commités:
  ```
  $ git stash
  ```
  Réappliquer les derniers changements mis dans la stash:
  ```
  $ git stash apply
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
