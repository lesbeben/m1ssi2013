package com.java.dataManager;

import java.io.StringWriter;
import java.io.UnsupportedEncodingException;
import java.security.InvalidKeyException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.List;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.SecretKeySpec;

import org.simpleframework.xml.ElementList;
import org.simpleframework.xml.Root;
import org.simpleframework.xml.Serializer;
import org.simpleframework.xml.core.Persister;

import android.content.Context;
import android.provider.Settings.Secure;

/**
 * Cette classe permet de charger et enregistrer les données Utilisateur.
 * 
 * @post tokenList != null
 * 		 decryptData(encryptData(data, key), key) == data
 *  
 * @author ADEGOLOYE Yves
 */

@Root
public final class LocalData {

	public static final String LOCAL_DATA_FILE = "localData.xml";
	public static final String LOCAL_PIN_FILE = "pin.xml";

	/**
	 * PIN de l'utisateur.
	 */
	private String pin = "0000";

	/**
	 * liste de token créés par l'utilisateur.
	 */
	@ElementList
	private List<Token> tokenList = new ArrayList<Token>();

	/**
	 * instance de la classe LocalData.
	 */
	private static LocalData instance = null;

	/**
	 * Constructeur de la classe LocalData.
	 */
	private LocalData() {
	}

	/**
	 * Cette fonction permet d'avoir une seule instance de la classe LocalData.
	 * Cette seule instance pourra être utilisée partout dans l'application.
	 * (design patern singleton)
	 */
	public static LocalData getInstance() {
		if (instance == null) {
			instance = new LocalData();
			return instance;
		}
		return instance;
	}

	/**
	 * Cette fonction retourne la liste des Tokens de l'utilisateur.
	 * 
	 * @return La liste des tokens de l'utilisateur.
	 */
	public List<Token> getListeToken() {
		return tokenList;
	}

	/**
	 * Cette fonction retourne un token en prenant comme paramètre son nom.
	 * 
	 * @param nom : Le nom du token à récupérer.
	 * 
	 * @pre nom != null
	 * 
	 * @return Le token récupéré s'il existe, null sinon.
	 */
	public Token getToken(String nom) {
		if (nom == null) {
			throw new IllegalArgumentException();
		}
		Token res = null;
		for (int i = 0; i < tokenList.size(); i++) {
			Token resIT = tokenList.get(i);
			if (resIT.getNom().equalsIgnoreCase(nom)) {
				res = resIT;
			}
		}
		return res;
	}

	/**
	 * Ajoute un token à la liste des tokens.
	 * 
	 * @param token : Le token à ajouter à la liste.
	 * 
	 * @pre token != null
	 * 		getToken(token.getNom()) == null
	 * @post getToken(token.getNom()) == token
	 */
	public void addToken(Token token) {
		if (token == null || getToken(token.getNom()) != null) {
			throw new IllegalArgumentException();
		}
		tokenList.add(token);
	}

	/**
	 * Supprime un token de la liste des tokens.
	 * 
	 * @param nom : Le nom du token à supprimer.
	 * 
	 * @pre nom != null
	 * @post getToken(nom) == null
	 */
	public void removeToken(String nom) {
		if (nom == null) {
			throw new IllegalArgumentException();
		}
		for (int i = 0; i < tokenList.size(); i++) {
			Token res = (Token) tokenList.get(i);
			if (res.getNom().equalsIgnoreCase(nom)) {
				tokenList.remove(i);
			}
		}
	}

	/**
	 * Supprime un token de la liste des tokens d'après sa position dans celle
	 * ci.
	 * 
	 * @param index dans la liste
	 * 
	 * @pre index > 0
	 * 		index < getTokenList.length
	 */
	public void removeToken(long index) {
		if (index <= 0) {
			throw new IllegalArgumentException();
		}
		tokenList.remove(index);
	}

	/**
	 * Cette fonction retourne le PIN de l'utiliateur.
	 * 
	 * @return Le pin de l'utilisateur
	 */
	public String getPIN() {
		return pin;
	}

	/**
	 * Modifie le PIN Utilisateur.
	 * 
	 * @param pin
	 * 
	 * @pre pin != null
	 * @post getPin == pin
	 */
	public void setPIN(String pin) {
		if (pin == null) {
			throw new IllegalArgumentException();
		}
		this.pin = pin;
	}

	/**
	 * Cette fonction permet de sérialiser cette classe en format XML.
	 * 
	 * @return Une serialisation de cette classe en XML
	 */
	private String serialize() {
		Serializer serializer = new Persister();
		StringWriter writer = new StringWriter();
		try {
			serializer.write(this, writer);
		} catch (Exception e) {
			// RIEN
		}
		return writer.toString();
	}

	/**
	 * Cette fonction permet de déserialiser un contenu de type LocalData.
	 * 
	 * @param contenuXML : La chaine au format XML à désérialiser.
	 * 
	 * @return LocalData Object
	 * @throws IOException
	 */
	private LocalData deserialize(String contenuXML) {
		Serializer serializer = new Persister();
		LocalData local = null;
		try {
			local = serializer.read(LocalData.class, contenuXML);
		} catch (Exception e) {
			// RIEN
		}
		return local;
	}

	/**
	 * cette fonction permet de chiffrer en AES les données passées en
	 * paramètres avec le PIN de l'utilisateur.
	 * 
	 * @param data : La chaine en clair non chiffrée
	 * @param aesKey : la clé pour le chiffrement
	 * 
	 * @pre data != null
	 * 		aesKey != null	
	 * 
	 * @return String correpondant au chiffrée AES de data.
	 */
	public byte[] encryptData(String data, byte[] aesKey) {
		if (data == null || aesKey == null) {
			throw new IllegalArgumentException();
		}
		
		Cipher cipher;
		try {
			cipher = Cipher.getInstance("AES");
			SecretKeySpec key;
			key = new SecretKeySpec(aesKey, "AES");
			cipher.init(Cipher.ENCRYPT_MODE, key);
			return cipher.doFinal(data.getBytes("UTF-8"));
		} catch (IllegalBlockSizeException e) {
			// TODO
		} catch (BadPaddingException e) {
			// TODO
		} catch (UnsupportedEncodingException e) {
			// TODO
		} catch (InvalidKeyException e) {
			// TODO
		} catch (NoSuchAlgorithmException e) {
			// TODO
		} catch (NoSuchPaddingException e) {
			// TODO
		}

		return null;

	}

	/**
	 * Cette fonction permet de déchiffrer les données (chiffrées avec AES)
	 * passées en paramétres avec la clef donnée.
	 * 
	 * @param data : Les données chiffrées sous forme de tableau d'octet.
	 * @param aesKey : La clé pour le déchiffrement
	 * 
	 * @return Le déchiffré AES de data pour la clef aesKey
	 */
	public String decryptData(byte[] data, byte[] aesKey) {
		Cipher cipher;
		try {
			cipher = Cipher.getInstance("AES");
			SecretKeySpec key;
			key = new SecretKeySpec(aesKey, "AES");
			cipher.init(Cipher.DECRYPT_MODE, key);
			return new String(cipher.doFinal(data), "UTF-8");
		} catch (IllegalBlockSizeException e) {
			// TODO
			return "Block Size";
		} catch (BadPaddingException e) {
			// TODO
			return "Padding";
		} catch (UnsupportedEncodingException e) {
			// TODO
			return "Encoding";
		} catch (InvalidKeyException e) {
			// TODO
			return "Key";
		} catch (NoSuchAlgorithmException e) {
			// TODO
			return "Algorithm";
		} catch (NoSuchPaddingException e) {
			// TODO
			return "Padding 2";
		}
	}

	/**
	 * Chargement des données utilisateurs.
	 * 
	 * @param context : Le contexte de l'application
	 * @pre context != null
	 */
	public void load(Context context) {
		// chargement du fichier "localData.xml"
		String res;
		byte[] contenu = IOFileUtils.readFromInternalFile(context,
				LOCAL_DATA_FILE);
		if (contenu != null) {
			res = decryptData(contenu, createAesKeyWithPin(context, pin));
			LocalData data = deserialize(res);
			if (data.getListeToken() != null) {
				tokenList = data.getListeToken();
			}
		}
	}

	/**
	 * Fonction utilisée pour l'enregistrement des données dans le fichier
	 * interne du device.
	 * 
	 * @param context : le contexte de l'application
	 * @pre context != null
	 * 		IOFileUtils.internalFileExists(LOCAL_DATA_FILE) == true
	 */
	public void commit(Context context) {

		// Enregistrement des données dans le fichier "localData.xml"
		String data = serialize();
		byte[] res;
		res = encryptData(data, createAesKeyWithPin(context, pin));
		IOFileUtils.clearFile(context, LOCAL_DATA_FILE);
		IOFileUtils.saveToInternalFile(context, LOCAL_DATA_FILE, res);
	}

	/**
	 * Fonction pour hasher le PIN en Sha1.
	 * 
	 * @param pin : Le pin à hacher.
	 * 
	 * @pre pin != null
	 * 
	 * @return Le pin hashé sous forme d'un tableau d'octets
	 */
	public byte[] hashPin(String pin) {
		byte[] bytes = null;
		try {
			// Hashage du PIN
			MessageDigest md = MessageDigest.getInstance("SHA1");
			md.update(pin.getBytes("UTF-8"));
			bytes = md.digest();
		} catch (NoSuchAlgorithmException e) {
			// TODO
		} catch (UnsupportedEncodingException e) {
			// TODO
		}
		return bytes;
	}

	/**
	 * Fonction utilisée pour enregistrer le PIN utilisateur Hashé en SHA1 dans
	 * le fichier pin.xml.
	 * 
	 * @param context : Le contexte de l'application
	 * 
	 * @pre context != null
	 * 		IOFileUtils.InternalFileExists(LOCAL_PIN_FILE) == true
	 */
	public void savePin(Context context) {
		// hashage du PIN
		byte[] bytes = hashPin(pin);

		// Enregistrement du hashage dans le fichier pin.xml
		IOFileUtils.clearFile(context, LOCAL_PIN_FILE);
		IOFileUtils.saveToInternalFile(context, LOCAL_PIN_FILE, bytes);
	}

	/**
	 * Fonction pour lire le contenu hashé du fichier pin.xml.
	 * 
	 * @param context : le contexte de l'application
	 * 
	 * @pre context != null
	 * 		IOFileUtils.internalFileExists(LOCAL_PIN_FILE) == true
	 * 
	 * @return Le hashé contenu dans le fichier LOCAL_PIN_FILE
	 */
	public byte[] loadPin(Context context) {
		// chargement du fichier "pin.xml"
		return IOFileUtils.readFromInternalFile(context, LOCAL_PIN_FILE);
	}

	/**
	 * Cette fonction compare le pin user lors du login et le pin hashé
	 * enregistrer localement.
	 * 
	 * @param pin : Le pin de l'utilisateur
	 * @param context : Le contexte de l'application
	 * 
	 * @pre context != null
	 * 		pin != null
	 * 		IOFileUtils.InternalFileExists(LOCAL_PIN_FILE)
	 * 
	 * @return true si le hashé du pin coincide avec le contenu de 
	 * 		   LOCAL_PIN_FILE
	 */
	public boolean validatePin(Context context, String pin) {
		return (new String(hashPin(pin))).equalsIgnoreCase(new String(
				loadPin(context)));
	}

	/**
	 * Fonction utilisée pour créer la clé AES (à base du PIN passé en
	 * paramètre) pour le chiffrement. La fonction de hashage utilisée est MD5
	 * avec une sortie de 128 bits
	 * 
	 * @param pin : Le PIN de l'utilisateur
	 * @param context : Le contexte de l'application
	 * 
	 * @pre context != null
	 * 		pin != null
	 * 
	 * @return Une chaine d'octet représentant une clef pour le chiffrement aes.
	 */
	public byte[] createAesKeyWithPin(Context context, String pin) {

		String androidId = Secure.getString(context.getContentResolver(),
				Secure.ANDROID_ID);

		String passwordToHash = pin + androidId;

		byte[] bytes = null;
		try {
			MessageDigest md = MessageDigest.getInstance("MD5");
			md.update(passwordToHash.getBytes());
			bytes = md.digest();
		} catch (NoSuchAlgorithmException e) {
			// TODO
		}
		return bytes;
	}

}
