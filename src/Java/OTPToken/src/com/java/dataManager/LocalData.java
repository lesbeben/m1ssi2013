package com.java.dataManager;

import java.io.IOException;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.List;

import org.simpleframework.xml.ElementList;
import org.simpleframework.xml.Root;
import org.simpleframework.xml.Serializer;
import org.simpleframework.xml.core.Persister;


/**
 * Cette classe permet de charger et enregistrer les données Utilisateur
 */

@Root
public class LocalData {
	
	
	
	/**
	 * PIN de l'utisateur
	 */
	private String PIN = "";


	
	
	/**
	 * liste de token créé par l'utilisateur
	 */
	@ElementList
	private List<Token> listeToken = new ArrayList<Token>();

	
	
	
	/**
	 * instance de la classe LocalData
	 */
	private LocalData instance = null;

	
	
	
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
	public LocalData getInstance() {
		if (instance == null) {
			return new LocalData();
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
		return null;
	}

	
	
	
	/**
	 * Ajoute un token à la liste des tokens
	 * 
	 * @param token
	 */
	public void addToken(Token token) {

	}

	
	
	
	/**
	 * Supprime un token de la liste des tokens
	 * 
	 * @param nom
	 */
	public void removeToken(String nom) {

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
		LocalData local=null;
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
	 * @return String chiffré avec AES
	 */
	public String EncryptData(String data){
		return null;
	}
	
	
	
	/**
	 * cette fonction permet de déchiffrer les données (chiffrées avec AES) passées en paramètres
	 * avec le PIN de l'utilisateur
	 * @param String chiffré
	 * @return String non chiffré
	 */
	public String DecryptData(String data){
		return null;
	}

}
