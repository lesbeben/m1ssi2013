package utils;

/**
 * Définit un type pour la génération de mots de passes jetables.
 * Ce type est muni d'une seule opération "generer()" permettant de générer un
 * mot de passe unique.
 * 
 * Selon le mode d'implantation de ce type, deux appels successifs à generer()
 * pourront avoir la même valeur. Il n'est cependant pas souhaitable que deux
 * appels séparés d'un certain temps retournent le même résultat.
 * 
 * @post (generer >= 0) && (generer < 100 000 000)
 * @author Gaëtan Ferry
 */
public interface IOTP {	
	/**
	 * Retourne la valeur du mot de passe jetable pour le contexte courant.
	 * 
	 * @return Un entier représentant un OTP 
	 */
	int generer();	
}
