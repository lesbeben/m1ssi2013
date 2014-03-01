package utils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;


/**
 * Une classe pour la génération de mots de
 * passes jetables avec r�implémentation du HMAC
 *
 * @author ADEGOLOYE Yves
 */
public final class Utils {
	
	public static final int BLK_SIZE = 64;
	private static final int IPAD = 0x36;
	private static final int OPAD = 0x5c;
	
	public static final int HASH_SIZE = 20;
	
	private Utils() {

	}

	/**
	 * Retourne la troncature de bytes selon l'algo donné dans la rfc 4226.
	 * Le bit de signe est éffacé pour éviter les incohérences dues au signe.
	 * 
	 * @param bytes
	 *            Une chaine d'octet de longueur 160 bits
	 * @return Une chaine de 32 bits avec le bit de poids fort à 0
	 */
	public static byte[] truncate(byte[] bytes) {
		if (bytes == null || bytes.length != HASH_SIZE) {
			throw new IllegalArgumentException();
		}
        // Récupération de l'offset
		int offset = (bytes[bytes.length - 1] & 0x0f);
		//Extraction de la bonne partie de la chaine
		byte[] res =  Arrays.copyOfRange(bytes, offset, offset + 4);
		//Effacement du bit de poids faible
		res[0] &= 0x7F;
		
		return res;
	}

	/**
	 * Retourne la valeur décimale correspondant à la conversion du binaire
	 * vers le décimal.
	 * 
	 * @pre bytes != null
	 * @param bytes
	 *            une chaine d'octets non null
	 * @return la conversion en décimal
	 */
	public static int convert(byte[] bytes) {
		if (bytes == null || bytes.length < 4) {
			throw new IllegalArgumentException();
		}
		int val;
        ByteBuffer bb = ByteBuffer.wrap(bytes);
        bb.order(ByteOrder.BIG_ENDIAN);
        val = bb.getInt();
		return val;
	}

	/**
	 * Calcule la valeur de HMAC SHA1.
	 * 
	 * @pre key != null
	 * @pre key.getSecret != null
	 * @pre count >= 0
	 * 
	 * @param key
	 *            la clef secrète
	 * @param count
	 *            le compteur pour le calcul
	 * @returnLa valeur de hmac_sha1(key, count)
	 */
	public static byte[] hmacSha1(ISecret key, long count) {
		if (key == null || key.getSecret() == null || count < 0) {
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
		if (key2Byte.length > BLK_SIZE) {
			sha1.update(key2Byte);
			key2Byte = sha1.digest();
			sha1.reset();
		}

		// complète la clé si nécessaire (taille < 64 ) voir RFC2104
		byte[] block = new byte[BLK_SIZE];
		for (int i = 0; i < key2Byte.length; ++i) {
			block[i] = key2Byte[i];
		}
		for (int i = key2Byte.length; i < block.length; ++i) {
			block[i] = 0;
		}

		// PREMIER-HASH = SHA-1(KEY ^ IPAD + MESSAGE)), IPAD = 64 bytes de 0x36.
		for (int i = 0; i < BLK_SIZE; ++i) {
			block[i] ^= IPAD;
		}
		sha1.update(block);
		ByteBuffer bb = ByteBuffer.allocate(8).putLong(count);
		bb.order(ByteOrder.BIG_ENDIAN);
		sha1.update(bb.array());
		byte[] hash = sha1.digest();
		sha1.reset();



		// DEUXIEME-HASH = SHA-1(KEY ^ OPAD + PREMIER_HASH), OPAD = 64 bytes de
		// 0x5c.
		for (int i = 0; i < BLK_SIZE; ++i) {
			block[i] ^=  (IPAD ^ OPAD);
		}
		sha1.update(block);
		sha1.update(hash);
		hash = sha1.digest();

		return hash;
	}
}
