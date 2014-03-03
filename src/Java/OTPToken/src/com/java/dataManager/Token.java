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
	private int method_Type = OTPMethodType.HOTP;
	
	
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
	private String skey_hex = "";
	

	
	/**
	 * Générateur otp.
	 */
	private IOTP iotp_gen ;

	
	/**
	 * constructeur du token.
	 * @return token
	 */
	public Token(String nom, int methodType, IOTP iotpGen) {

		this.nom = nom;
		this.method_Type = methodType;
		this.iotp_gen = iotpGen;
		this.tailleOTP = ((OTPGenerator) iotpGen).getDigits();
		this.skey_hex = 
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
	public int getMethod_Type() {
		return method_Type;
	}


    /**
     * Cette fonction retourne le générateur de token utilisé (HOTP ou TOTP).
     * 
     * @return IOTP
     */
	public IOTP getIotp_gen() {
		return iotp_gen;
	}
	
	
	
	/**
	 * c'est une fonction propre au token qui appelle implicitement son 
	 * générateur IOTP.
	 * 
	 * @return
	 */
	public int generate() {
		if (method_Type == OTPMethodType.HOTP) {
			if (iotp_gen == null) {
				Secret secret = new Secret();
				secret.setSecret(skey_hex);
				iotp_gen = new HOTP(count, secret, tailleOTP);
			}
		} else {
			if (iotp_gen == null) {
				Secret secret = new Secret();
				secret.setSecret(skey_hex);
				iotp_gen = new TOTP(secret, tailleOTP, quantum);
			}
		}

		int resultat = iotp_gen.generer();
		
		if (method_Type == OTPMethodType.HOTP) {
        count = ((OTPGenerator) iotp_gen).getCount();
		}	
		return resultat;
	}

}
