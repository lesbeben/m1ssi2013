package com.java.dataManager;

import org.simpleframework.xml.Element;
import org.simpleframework.xml.Root;
import com.java.utils.HOTP;
import com.java.utils.IOTP;
import com.java.utils.OTPGenerator;
import com.java.utils.Secret;
import com.java.utils.TOTP;

/**
 * Générateur de token.
 * Attributs: - nom du Token
 *            - clé de Génération
 *            - méthode de Génération (HOTP/TOTP)
 *            - longueur du Token
 * @author ADEGOLOYE Yves
 */

@Root
public class Token  {

	/**
	 * Nom du Token.
	 */
	@Element
	private String nom = "";

	/**
	 * type de génération utilisé (par défaut HOTP).
	 */
	@Element
	private OTPMethodType methodType = OTPMethodType.HOTP;
	
	/**
	 * Taille de l'OTP.
	 */
	@Element
	private int tailleOTP = 0;	
	
	/**
	 * Compteur actuel du token si méthod_type=HOTP, 0 sinon.
	 */
	@Element
	private long count = 0;	
	
	/**
	 * Temps de validité du token si méthod_type = TOTP, 0 sinon.
	 */
	@Element
	private int quantum = 0;	
	
	/**
	 * Clé de hashage utilisée.
	 */
	@Element
	private String skeyHex = "";
	
	/**
	 * Générateur otp.
	 */
	private IOTP iotpGen;
	
	/**
	 * constructeur du token.
	 * @return token
	 */
	public Token(String nom, OTPMethodType methodType, IOTP iotpGen) {

		this.nom = nom;
		this.methodType = methodType;
		this.iotpGen = iotpGen;
		this.tailleOTP = ((OTPGenerator) iotpGen).getDigits();
		this.skeyHex = 
				((OTPGenerator) iotpGen).getKey().getHexRepresentation();
	}
	
	
	
	/**
	 * Constructeur par défaut (utille pour la désérialisation).
	 */
	public Token() {	
	}
	

    /**
     * Cette fonction retourne le nom du token.
     * 
     * @return nom
     */
	public String getNom() {
		return nom;
	}

	
    /**
     * Cette fonction retourne le type de générateur token utilisé (HOTP ou 
     * TOTP).
     * 
     * @return O pour HOTP
     * @return 1 pour TOTP
     */
	public OTPMethodType getMethodType() {
		return methodType;
	}


    /**
     * Cette fonction retourne le générateur de token utilisé (HOTP ou TOTP).
     * 
     * @return IOTP
     */
	public IOTP getIotpGen() {
		return iotpGen;
	}
	
	
	
	/**
	 * c'est une fonction propre au token qui appelle implicitement son 
	 * générateur IOTP.
	 * 
	 * @return
	 */
	public int generate() {
		if (methodType == OTPMethodType.HOTP) {
			if (iotpGen == null) {
				Secret secret = new Secret();
				secret.setSecret(skeyHex);
				iotpGen = new HOTP(count, secret, tailleOTP);
			}
		} else {
			if (iotpGen == null) {
				Secret secret = new Secret();
				secret.setSecret(skeyHex);
				iotpGen = new TOTP(secret, tailleOTP, quantum);
			}
		}

		int resultat = iotpGen.generer();
		
		if (methodType == OTPMethodType.HOTP) {
        count = ((OTPGenerator) iotpGen).getCount();
		}	
		return resultat;
	}
}
