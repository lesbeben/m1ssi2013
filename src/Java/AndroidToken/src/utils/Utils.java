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
		// TODO Corp bidon pour faire plaisir a Eclipse.
		return new byte[0];
	}

	/**
	 * Retourne la valeure décimal conrespondant à la conversion du binaire
	 * vers le décimal.
	 * 
	 * @param bytes
	 *            une chaine de d'octet non null
	 * @return la conversion en décimal
	 */
	public static int convert(byte[] bytes) {
		DataInput input = new DataInputStream(new ByteArrayInputStream(bytes,
				0, bytes.length));
		int val;
		try {
			val = input.readInt();
		} catch (IOException e) {
			throw new IllegalArgumentException(e);
		}

		return val;
	}

	/**
	 * Calcul la valeure de HMAC SHA1.
	 * 
	 * @return
	 */
	public static byte[] hmacSha1() {
		// TODO corps absent
		return new byte[0];
	}
}
