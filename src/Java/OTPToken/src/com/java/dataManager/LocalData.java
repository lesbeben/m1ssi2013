package com.java.dataManager;

import java.io.IOException;
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
import com.java.dataManager.IOFileUtils;
import com.java.dataManager.Token;


/**
 * Cette classe permet de charger et enregistrer les données Utilisateur
 * 
 * @author ADEGOLOYE Yves
 */

@Root
public class LocalData {
	
	public static String LOCAL_DATA_FILE = "localData.xml";
	public static String LOCAL_PIN_FILE = "pin.xml";
	
	
	/**
	 * PIN de l'utisateur
	 */
	private String PIN = "0000";


	
	
	/**
	 * liste de token créé par l'utilisateur
	 */
	@ElementList
	private List<Token> listeToken = new ArrayList<Token>();

	
	
	
	/**
	 * instance de la classe LocalData
	 */
	private static LocalData instance = null;

	
	
	
	/**
	 * constructeur de la classe LocalData
	 */
	private LocalData() {
	}

	
	
	/**
	 * Cette fonction permet d'avoir une seule instance de la classe LocalData
	 * cette seule instance pourra être utilisée partout dans l'application
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
	 * Cette fonction retourne la liste des Tokens de l'utilisateur
	 * 
	 * @return listeToken
	 */
	public List<Token> getListeToken() {
		return listeToken;
	}

	
	
	
	/**
	 * Cette fonction retourne un token en prenant comme paramètre son nom
	 * 
	 * @param nom
	 * @return token
	 */
	public Token getToken(String nom) {
		Token res = null;
		for (int i = 0; i < listeToken.size(); i++) {
			Token resIT = listeToken.get(i);
			if (resIT.getNom().equalsIgnoreCase(nom)) {
				res = resIT;
			}
		}
		return res;
	}

	
	
	
	/**
	 * Ajoute un token à la liste des tokens
	 * 
	 * @param token
	 */
	public void addToken(Token token) {
		listeToken.add(token);
	}

	
	
	
	/**
	 * Supprime un token de la liste des tokens
	 * 
	 * @param nom
	 */
	public void removeToken(String nom) {
		for (int i = 0; i < listeToken.size(); i++) {
			Token res = (Token) listeToken.get(i);
			if (res.getNom().equalsIgnoreCase(nom)) {
				listeToken.remove(i);
			}
		}
	}
	
	
	/**
	 * Cette fonction retourne le PIN de l'utiliateur
	 * 
	 * @return PIN
	 */
	public String getPIN() {
		return PIN;
	}
	
	
	

	/**
	 * Modifie le PIN Utilisateur
	 * 
	 * @param pIN
	 */
	public void setPIN(String pIN) {
		PIN = pIN;
	}

	
	
	
	/**
	 * Cette fonction permet de sérialiser cette classe en format XML
	 * @return StringXML
	 */
	private String serialize() {
		Serializer serializer = new Persister();
		StringWriter writer = new StringWriter();
		try {
			serializer.write(this, writer);
		} catch (Exception e) {
		}
		return writer.toString();
	}

	
	
	
	/**
	 * Cette fonction permet de déserialiser un contenu de type LocalData
	 * @throws IOException
	 * @return LocalData Object
	 */
	private LocalData deserialize(String contenuXML) {
		Serializer serializer = new Persister();
		LocalData local = null;
		try {
			local = serializer.read(LocalData.class, contenuXML);
		} catch (Exception e) {
		}
		return local;
	}
	
	
	
	/**
	 * cette fonction permet de chiffrer en AES les données passées en paramètres
	 * avec le PIN de l'utilisateur
	 * @param String non chiffré
	 * @param byte[] la clé pour le chiffrement
	 * @return String chiffré avec AES
	 */
	private byte[] EncryptData(String data,byte[] key_AES) {

		Cipher cipher;
		try {
			cipher = Cipher.getInstance("AES");
			SecretKeySpec key;
			key = new SecretKeySpec(key_AES, "AES");
			cipher.init(Cipher.ENCRYPT_MODE, key);
			return cipher.doFinal(data.getBytes("UTF-8"));
		} catch (IllegalBlockSizeException e) {
		} catch (BadPaddingException e) {
		} catch (UnsupportedEncodingException e) {
		} catch (InvalidKeyException e) {
		} catch (NoSuchAlgorithmException e) {
		} catch (NoSuchPaddingException e) {
		}

		return null;

	}
	
	
	
	/**
	 * cette fonction permet de déchiffrer les données (chiffrées avec AES) passées en paramètres
	 * avec le PIN de l'utilisateur
	 * @param String chiffré
	 * @param byte[] la clé pour le déchiffrement, le même utilisé lors du chiffrement
	 * @return String non chiffré
	 */
	public String DecryptData(byte[] data,byte[] key_AES){
		Cipher cipher;
		try {
			cipher = Cipher.getInstance("AES");
			SecretKeySpec key;
			key = new SecretKeySpec(key_AES, "AES");
			cipher.init(Cipher.DECRYPT_MODE, key);
			return new String(cipher.doFinal(data), "UTF-8");
		} catch (IllegalBlockSizeException e) {
		} catch (BadPaddingException e) {
		} catch (UnsupportedEncodingException e) {
		} catch (InvalidKeyException e) {
		} catch (NoSuchAlgorithmException e) {
		} catch (NoSuchPaddingException e) {
		}

		return null;
	}
	
	
	/**
	 * chargement des données utilisateurs
	 * @param le context de l'application
	 */
	public void load(Context context) {

		// chargement du fichier "localData.xml"
		String res;
		try {
			res = DecryptData(IOFileUtils.readFromInternalFile(context,
					LOCAL_DATA_FILE),createAESKey_withPIN(context, PIN));
			LocaleDataTest data = deserialize(res);
			listeToken = data.getListeToken();
		} catch (UnsupportedEncodingException e) {
		}
	}
	
	
	/**
	 * fonction utilisée pour l'enregistrement des données dans le fichier interne du device
	 * @param le contexte de l'application
	 */
	public void commit(Context context) {

		// Enregistrement des données dans le fichier "localData.xml"
		String data = serialize();
		byte[] res;
		try {
			res = EncryptData(data,createAESKey_withPIN(context, PIN));
			IOFileUtils.clearFile(context, LOCAL_DATA_FILE);
			IOFileUtils.saveToInternalFile(context, LOCAL_DATA_FILE, res);
		} catch (UnsupportedEncodingException e) {
		}
	}

	/**
	 * fonction pour hashé le PIN en Sha1
	 * 
	 * @param pin non hashé
	 * @return byte[] pin hashé
	 */
	public byte[] hash_pin(String pin) {

		byte[] bytes = null;
		try {

			// Hashage du PIN
			MessageDigest md = MessageDigest.getInstance("SHA1");
			md.update(pin.getBytes("UTF-8"));
			bytes = md.digest();
			
		} catch (NoSuchAlgorithmException e) {
		} catch (UnsupportedEncodingException e) {
		}
		return bytes;

	}

	/**
	 * fonction utilisée pour enregistrer le PIN utilisateur Hashé en SHA1 dans
	 * le fichier pin.xml
	 * 
	 * @param le contexte de l'application
	 */
	public void savePin(Context context) {

		// hashage du PIN
		byte[] bytes = hash_pin(PIN);

		// Enregistrement du hashage dans le fichier pin.xml
		IOFileUtils.clearFile(context, LOCAL_PIN_FILE);
		IOFileUtils.saveToInternalFile(context, LOCAL_PIN_FILE, bytes);
	}

	/**
	 * fonction pour lire le contenu hashé du fichier pin.xml
	 * 
	 * @param lecontexte de l'application
	 * @return PIN hashé
	 */
	public byte[] loadPin(Context context) {

		// chargement du fichier "pin.xml"
		byte[] hashpin = IOFileUtils.readFromInternalFile(context,
				LOCAL_PIN_FILE);
		return hashpin;
	}
	
	
	/**
	 * fonction utilisée pour créer la clé AES (à base du PIN passé en paramètre)
	 * pour le chiffrement.
	 * La fonction de hashage utilisée est MD5 avec une sortie de 128 bits
	 * @param PIN de l'utilisateur
	 * @param contexte de l'application 
	 */
	private byte[] createAESKey_withPIN(Context context,String PIN){
		
		String android_id = Secure.getString(context.getContentResolver(),
                Secure.ANDROID_ID); 
		
		String passwordToHash = PIN + android_id;
		
        byte[] bytes = null;
        try {
        	
            MessageDigest md = MessageDigest.getInstance("MD5");
            
            md.update(passwordToHash.getBytes());
            
             bytes = md.digest();
            
        } 
        catch (NoSuchAlgorithmException e) 
        {
        }
        return bytes;
	}

}
