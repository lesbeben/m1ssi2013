package com.java.utils;

/**
 * Définit un type pour la génération de mots de passes jetables.
 * @author celtic
 */
public interface IOTP {	
	/**
	 * Retourne la valeur du mot de passe jetable pour le contexte courant.
	 * @pre La clef n'est pas nulle
	 * @param offset un éventuel décalage qui sera prit en compte dans le calcul
	 * 		  de l'OTP. Ce paramètre ne sera pas utilisé pour les générateurs 
	 * 		  HOTP.
	 * @return Un entier de 6 à 8 chiffres représentant un OTP 
	 */
	int generer(long offset);	
}
