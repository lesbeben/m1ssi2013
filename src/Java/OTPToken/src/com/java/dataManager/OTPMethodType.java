package com.java.dataManager;

/**
 * Interface catégorisant les 2 types de génération d'OTP utilisées.
 * 
 * @author ADEGOLOYE Yves
 */
public enum OTPMethodType {
	HOTP, TOTP;
	
	private OTPMethodType() {
		//Pour usage ultérieur
	}
}
