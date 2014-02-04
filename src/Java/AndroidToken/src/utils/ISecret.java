package utils;
/**
 * Une classe pour la gestion des clefs secrètes.
 * 
 * @post length(getSecret()) == this.length 
 * @post length(getHexRepresentation) == 2 * this.length 
 * @post getSecret() = hexToBinary(getHexRepresentation)
 * @author celtic
 *
 */
public interface ISecret {
	
	/**
	 * Initialise le secret avec la valeur correspondant a secret.
	 * @pre secret.length > 0
	 * @post getSecret = secret
	 * @param secret La valeur en octet du secret.
	 */
	void setSecret(byte[] secret);
	/**
	 * Initialise le secret avec une valeur aléatoire de longueur length.
	 * @pre length > 0
	 * @post getLength = length
	 * @param length la longueure du secret souhaité.
	 */
	void setSecret(int length);	
	/**
	 * Initialise le secret depuis une chaine hexadécimale.
	 * @pre hexRepresentation != null 
	 * @post getHexRepresentation = hexRep
	 * @param hexRepresentation la chaine hexa représentant le secret.
	 */
	void setSecret(String hexRepresentation);
	
	/**
	 * La longueur du secret courant en octet.
	 * @pre getSecret != null
	 * @return La longueure du secret.
	 */
	int getLength();	
	/**
	 * La valeur du secret courant en tableau d'octet.
	 * @return Un tableau d'octet contenant la valeur du secret.
	 */
	byte[] getSecret();
	/**
	 * Une représentation human readable du secret.
	 * 
	 * @pre getSecret != null
	 * @return Une chaine de caractères héxadecimaux représentant la valeur 
	 * 		   du secret courant.
	 */
	String getHexRepresentation();
}
