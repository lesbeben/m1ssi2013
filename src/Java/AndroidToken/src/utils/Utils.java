package utils;

import java.io.ByteArrayInputStream;
import java.io.DataInput;
import java.io.DataInputStream;
import java.io.IOException;
import java.security.MessageDigest;

public final class Utils {
	private Utils() {

	}

	/**
	 * Retourne la toncature de bytes selon l'algo donné dans la rfc 4226.
	 * 
	 * @param bytes
	 *            Une chaine d'octet de longueure 160 bits
	 * @return Une chaine de 31 bits
	 */
	public static byte[] truncate(byte[] bytes) {
		if (bytes == null) {
			throw new IllegalArgumentException();
		}
		// TODO Corp bidon pour faire plaisir a Eclipse.
		return new byte[0];
	}

	/**
	 * Retourne la valeure décimal conrespondant à la conversion du binaire
	 * vers le décimal.
	 * @pre bytes != null
	 * @param bytes
	 *            une chaine de d'octet non null
	 * @return la conversion en décimal
	 */
	public static int convert(byte[] bytes) {
		int val;
		try {
			DataInput input = new DataInputStream(new ByteArrayInputStream(
					bytes, 0, bytes.length));
			val = input.readInt();
		} catch (IOException e) {
			throw new IllegalArgumentException(e);
		}

		return val;
	}

	/**
	 * Calcul la valeure de HMAC SHA1.
	 * @pre key != null
	 * @pre key.getSecret != null
	 * @pre count >= 0
	 * 
	 * @param key la clef secrete
	 * @param count le conteur pour le calcul
	 * @returnLa valeur de hmac_sha1(key, count)
	 */
	public static byte[] hmacSha1(ISecret key, long count) {
		// TODO corps absent
		return new byte[0];
	}
}
