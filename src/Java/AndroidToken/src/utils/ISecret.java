package utils;
/**
 * Une classe pour la gestion des clefs secrètes.
 * Elle permet de gérer de manière simple les secrets cryptographiques de 
 * longueure quelconque. 
 * Les ISecret sont initialisable avec une chaine de caractère hexadécimaux
 * ou directement une chaine d'octet.
 * On dispose aussi d'un moyen pour générer une valeur aléatoire à partir de 
 * rien.
 * 
 * @post length(getSecret()) == this.length 
 * @post length(getHexRepresentation) == 2 * this.length 
 * @post getSecret() = hexToBinary(getHexRepresentation)
 * @author celtic
 *
 */
public interface ISecret {
	
	/**
	 * Initialise le secret avec la valeur correspondant a la chaine de bit 
	 * secret.
	 * 
	 * @pre secret.length > 0
	 * @pre secret != null
	 * @post getSecret = secret
	 * @param secret La valeur en octet du secret.
	 */
	void setSecret(byte[] secret);
	/**
	 * Initialise le secret avec une valeur aléatoire de longueur length.
	 * La valeur retournée doit être le plus aléatoire possible.
	 * 
	 * @pre length > 0
	 * @post getLength = length
	 * @param length la longueure du secret souhaité.
	 */
	void setSecret(int length);	
	/**
	 * Initialise le secret depuis une chaine hexadécimale. 
	 * La valeur du secret sera égal à la valeur représentée par la chaine.
	 * 
	 * @pre hexRepresentation != null 
	 * @post getHexRepresentation = hexRep
	 * @param hexRepresentation la chaine hexa représentant le secret.
	 */
	void setSecret(String hexRepresentation);
	
	/**
	 * La longueur du secret courant en octet.
	 * 
	 * @pre getSecret != null
	 * @return La longueure du secret.
	 */
	int getLength();	
	/**
	 * La valeur du secret courant sous la forme d'un tableau d'octet.
	 * 
	 * @return Un tableau d'octet contenant la valeur du secret.
	 */
	byte[] getSecret();
	/**
	 * Une représentation human readable du secret.
	 * La représentation est donnée sous la forme d'une chaine de caractère 
	 * héxadécimaux.
	 * 
	 * @pre getSecret != null
	 * @return Une chaine de caractères héxadecimaux représentant la valeur 
	 * 		   du secret courant.
	 */
	String getHexRepresentation();
}
