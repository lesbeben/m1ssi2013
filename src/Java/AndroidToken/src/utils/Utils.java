package utils;

import java.security.MessageDigest;

public final class Utils {
	private Utils() {
		
	}
	
	/**
	 * Retourne la toncature de bytes selon l'algo donné dans la rfc 4226.
	 * @param bytes Une chaine d'octet de longueure 160 bits
	 * @return Une chaine de 31 bits
	 */
	public static byte[] truncate(byte[] bytes) {
		//TODO Corp bidon pour faire plaisir a Eclipse.
		return new byte[0];
	}

	/**
	 * Retourne la valeure décimal conrespondant à la conversion du binaire
	 * vers le décimal.
	 *
	 * @param bytes une chaine de d'octet non null
	 * @return la conversion en décimal
	 */
	public static int convert(byte[] bytes) {
		//TODO corp absent
		return 0;
	}
	
	/**
	 * Calcul la valeure de HMAC SHA1.
	 * @return
	 */
	public static byte[] hmacSha1() {
		//TODO corps absent
		return new byte[0];
	}
}
