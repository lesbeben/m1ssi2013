package com.java.dataManager;

import org.simpleframework.xml.Element;
import org.simpleframework.xml.Root;
import com.utils.IOTP;
import com.utils.OTPGenerator;



/**Générateur token
 Attributs: -nom du Token
 -clé de Génération
 -méthode de Génération (HOTP/TOTP)
 -longueur du Token
 */

@Root
public class Token  {

	/**
	 * nom du Token
	 */
	@Element
	private String nom = "";

	/**
	 * type de génération utilisé (par défaut HOTP)
	 */
	@Element
	private int method_Type = OTPMethodType.HOTP;
	
	
	/**
	 * taille de l'OTP
	 */
	@Element
	private int tailleOTP=0; 
	
	
	/**
	 * compteur actuel du token si méthod_type=HOTP, 0 sinon
	 */
	@Element
	private long count=0;
	
	
	
	/**
	 * clé de hashage utilisée
	 */
	@Element
	private String skey_hex = "";
	

	
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
		this.tailleOTP= ((OTPGenerator)iotp_gen).getDigits();
		this.skey_hex= ((OTPGenerator)iotp_gen).getKey().getHexRepresentation();
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
	 * c'est une fonction propre au token qui appelle implicitement son générateur IOTP
	 * @return
	 */
	public int generate(){
		int resultat = iotp_gen.generer();
		if(method_Type==OTPMethodType.HOTP){
        count =((OTPGenerator)iotp_gen).getCount();
		}	
		return resultat;
	}

}
