package com.java.dataManager;

/**
 * Enumération représentant les types de générateurs d'OTP disponibles.
 * 
 * @author ADEGOLOYE Yves
 */
public enum OTPMethodType {
	HOTP, TOTP;
	
	private OTPMethodType() {
		//Pour usage ultérieur
	}
}
