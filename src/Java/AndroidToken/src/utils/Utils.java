package utils;

import java.io.ByteArrayInputStream;
import java.io.DataInput;
import java.io.DataInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public final class Utils {
	private Utils() {

	}

	/**
	 * Retourne la toncature de bytes selon l'algo donnÃ© dans la rfc 4226.
	 * 
	 * @param bytes
	 *            Une chaine d'octet de longueure 160 bits
	 * @return Une chaine de 31 bits
	 */
	public static byte[] truncate(byte[] bytes) {
		if (bytes == null) {
			throw new IllegalArgumentException();
		}
        // rfc 4226
		int offset = bytes[bytes.length - 1] & 0xf;
		int binary = ((bytes[offset] & 0x7f) << 24)
				| ((bytes[offset + 1] & 0xff) << 16)
				| ((bytes[offset + 2] & 0xff) << 8)
				| (bytes[offset + 3] & 0xff);
		return ByteBuffer.allocate(8).putInt(binary).array();
	}

	/**
	 * Retourne la valeure dÃ©cimal conrespondant Ã  la conversion du binaire
	 * vers le dÃ©cimal.
	 * 
	 * @pre bytes != null
	 * @param bytes
	 *            une chaine de d'octet non null
	 * @return la conversion en dÃ©cimal
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
	 * 
	 * @pre key != null
	 * @pre key.getSecret != null
	 * @pre count >= 0
	 * 
	 * @param key
	 *            la clef secrete
	 * @param count
	 *            le conteur pour le calcul
	 * @returnLa valeur de hmac_sha1(key, count)
	 */
	public static byte[] hmacSha1(ISecret key, long count) {
		if (key == null) {
			throw new IllegalArgumentException();
		}

		byte[] key2Byte = key.getSecret();

		// permet de recuperer le generateur SHA-1
		MessageDigest sha1 = null;
		try {
			sha1 = MessageDigest.getInstance("SHA1");
		} catch (NoSuchAlgorithmException e) {
			throw new java.lang.AssertionError(
					".hmacSHA1(): SHA-1 algorithm not found!");
		}

		// hash la clé si necessaire si sa taille > 64 voir RFC2104
		if (key2Byte.length > 64) {
			sha1.update(key2Byte);
			key2Byte = sha1.digest();
			sha1.reset();
		}

		// complète la clé si nécessaire (taille < 64 ) voir RFC2104
		byte block[] = new byte[64];
		for (int i = 0; i < key2Byte.length; ++i)
			block[i] = key2Byte[i];
		for (int i = key2Byte.length; i < block.length; ++i)
			block[i] = 0;

		// PREMIER-HASH = SHA-1(KEY ^ IPAD + MESSAGE)), IPAD = 64 bytes de 0x36.
		for (int i = 0; i < 64; ++i)
			block[i] ^= 0x36;
		sha1.update(block);
		sha1.update(ByteBuffer.allocate(8).putLong(count).array());
		byte[] hash = sha1.digest();
		sha1.reset();

		// DEUXIEME-HASH = SHA-1(KEY ^ OPAD + PREMIER_HASH), OPAD = 64 bytes de
		// 0x5c.
		for (int i = 0; i < 64; ++i)
			block[i] ^= (0x36 ^ 0x5c);
		sha1.update(block);
		sha1.update(hash);
		hash = sha1.digest();

		return hash;
	}
}
