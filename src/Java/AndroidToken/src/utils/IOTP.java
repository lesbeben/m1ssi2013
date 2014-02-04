package utils;

/**
 * Définit un type pour la génération de mots de passes jetables.
 * @author celtic
 */
public interface IOTP {	
	/**
	 * Retourne la valeur du mot de passe jetable pour le contexte courant.
	 * @pre La clef n'est pas nulle
	 * @return Un entier de 6 à 8 chiffres représentant un OTP 
	 */
	int generer();	
}
