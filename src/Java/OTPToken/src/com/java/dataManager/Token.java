package com.java.dataManager;

import com.utils.IOTP;

/**Générateur token
 Attributs: -nom du Token
 -clé de Génération
 -méthode de Génération (HOTP/TOTP)
 -longueur du Token
 */

public class Token  {

	/**
	 * nom du Token
	 */
	private String nom = "";

	/**
	 * méthode de génération à utiliser (par défaut HOTP)
	 */
	private int method_Type = OTPMethodType.HOTP;

	
	/**
	 * générateur otp
	 */
	private IOTP iotp_gen ;

	
	/**
	 * constructeur du token 
	 * @return token
	 */
	public void token(String nom, int method_type, IOTP iotp_gen) {

		this.nom = nom;
		this.method_Type = method_type;
		this.iotp_gen = iotp_gen;
	}

    /**
     * Cette fonction retourne le nom du token
     * @return nom
     */
	public String getNom() {
		return nom;
	}

	
    /**
     * Cette fonction retourne le type de gérérateur token
     * utilisé (HOTP ou TOTP)
     * @return O pour HOTP
     * @return 1 pour TOTP
     */
	public int getMethod_Type() {
		return method_Type;
	}


    /**
     * Cette fonction retourne le gérérateur token
     * utilisé (HOTP ou TOTP)
     * @return IOTP
     */
	public IOTP getIotp_gen() {
		return iotp_gen;
	}
	
	
	/**
	 * cette fonction toString() retournera cet objet Token 
	 * sous forme de chaine de caractère
	 * @return String
	 */
	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return super.toString();
	}

}
