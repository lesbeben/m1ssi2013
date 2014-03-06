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
	private IOTP otpGen;
	
	/**
	 * constructeur du token.
	 * @pre nom != null
	 * 		methodType != null
	 * 		otpGen != null
	 * 		methodType == otpGen.class
	 * @return token
	 */
	public Token(String nom, OTPMethodType methodType, IOTP otpGen) {
		if (nom == null || methodType == null || otpGen == null) {
			throw new IllegalArgumentException();
		}
		if (methodType == OTPMethodType.HOTP) {
			if (otpGen.getClass() != HOTP.class) {
				throw new IllegalArgumentException();
			}
		} else {
			if (otpGen.getClass() != TOTP.class) {
				throw new IllegalArgumentException();
			}
		}
		this.nom = nom;
		this.methodType = methodType;
		this.otpGen = otpGen;
		this.tailleOTP = ((OTPGenerator) otpGen).getDigits();
		this.skeyHex = 
				((OTPGenerator) otpGen).getKey().getHexRepresentation();
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
		return otpGen;
	}
	
	
	
	/**
	 * c'est une fonction propre au token qui appelle implicitement son 
	 * générateur IOTP.
	 * 
	 * @return
	 */
	public int generate() {
		if (methodType == OTPMethodType.HOTP) {
			if (otpGen == null) {
				Secret secret = new Secret();
				secret.setSecret(skeyHex);
				otpGen = new HOTP(count, secret, tailleOTP);
			}
		} else {
			if (otpGen == null) {
				Secret secret = new Secret();
				secret.setSecret(skeyHex);
				otpGen = new TOTP(secret, tailleOTP, quantum);
			}
		}

		int resultat = otpGen.generer();
		
		if (methodType == OTPMethodType.HOTP) {
			count = ((OTPGenerator) otpGen).getCount();
		}	
		return resultat;
	}
}
